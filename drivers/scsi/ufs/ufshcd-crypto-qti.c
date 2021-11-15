// SPDX-License-Identifier: GPL-2.0-only
/*
 * UFS Crypto ops QTI implementation.
 *
 * Copyright (c) 2020-2021, Linux Foundation. All rights reserved.
 */

#include <crypto/algapi.h>
#include <linux/platform_device.h>
#include <linux/crypto-qti-common.h>

#include "ufshcd-crypto-qti.h"

#define MINIMUM_DUN_SIZE 512
#define MAXIMUM_DUN_SIZE 65536

#define NUM_KEYSLOTS(hba) (hba->crypto_capabilities.config_count + 1)

/* Blk-crypto modes supported by UFS crypto */
static const struct ufs_crypto_alg_entry {
	enum ufs_crypto_alg ufs_alg;
	enum ufs_crypto_key_size ufs_key_size;
} ufs_crypto_algs[BLK_ENCRYPTION_MODE_MAX] = {
	[BLK_ENCRYPTION_MODE_AES_256_XTS] = {
		.ufs_alg = UFS_CRYPTO_ALG_AES_XTS,
		.ufs_key_size = UFS_CRYPTO_KEY_SIZE_256,
	},
};

static uint8_t get_data_unit_size_mask(unsigned int data_unit_size)
{
	if (data_unit_size < MINIMUM_DUN_SIZE ||
		data_unit_size > MAXIMUM_DUN_SIZE ||
	    !is_power_of_2(data_unit_size))
		return 0;

	return data_unit_size / MINIMUM_DUN_SIZE;
}

void ufshcd_crypto_qti_enable(struct ufs_hba *hba)
{
	int slot;
	int err = 0;

	if (!(hba->caps & UFSHCD_CAP_CRYPTO))
		return;

	err = crypto_qti_enable(hba->priv);
	if (err) {
		pr_err("%s: Error enabling crypto, err %d\n",
				__func__, err);
		ufshcd_crypto_qti_disable(hba);
	}

	hba->caps |= UFSHCD_CAP_CRYPTO;
	/* Clear all keyslots */
	for (slot = 0; slot < hba->ksm.num_slots; slot++)
		hba->ksm.ksm_ll_ops.keyslot_evict(&hba->ksm, NULL, slot);
}

void ufshcd_crypto_qti_disable(struct ufs_hba *hba)
{
	hba->caps &= ~UFSHCD_CAP_CRYPTO;
	crypto_qti_disable(hba->priv);
}


static int ufshcd_crypto_qti_keyslot_program(struct blk_keyslot_manager *ksm,
					     const struct blk_crypto_key *key,
					     unsigned int slot)
{
	struct ufs_hba *hba = container_of(ksm, struct ufs_hba, ksm);
	const union ufs_crypto_cap_entry *ccap_array = hba->crypto_cap_array;
	const struct ufs_crypto_alg_entry *alg =
			&ufs_crypto_algs[key->crypto_cfg.crypto_mode];
	int err = 0;
	int i;
	u8 data_unit_mask;
	int crypto_alg_id = -1;

	data_unit_mask = get_data_unit_size_mask(key->crypto_cfg.data_unit_size);
	for (i = 0; i < hba->crypto_capabilities.num_crypto_cap; i++) {
		if (ccap_array[i].algorithm_id == alg->ufs_alg &&
		    ccap_array[i].key_size == alg->ufs_key_size &&
		    (ccap_array[i].sdus_mask & data_unit_mask)) {
			crypto_alg_id = i;
			break;
		}
	}

	if (WARN_ON(crypto_alg_id < 0))
		return -EOPNOTSUPP;

	if (!(data_unit_mask &
	      hba->crypto_cap_array[crypto_alg_id].sdus_mask))
		return -EINVAL;

	if (!hba->pm_op_in_progress)
		pm_runtime_get_sync(hba->dev);
	err = ufshcd_hold(hba, false);
	if (err) {
		pr_err("%s: failed to enable clocks, err %d\n", __func__, err);
		goto out;
	}

	err = crypto_qti_keyslot_program(hba->priv, key, slot,
					data_unit_mask, crypto_alg_id);
	if (err)
		pr_err("%s: failed with error %d\n", __func__, err);

	ufshcd_release(hba);
out:
	if (!hba->pm_op_in_progress)
		pm_runtime_put_sync(hba->dev);

	return err;
}

static int ufshcd_crypto_qti_keyslot_evict(struct blk_keyslot_manager *ksm,
					   const struct blk_crypto_key *key,
					   unsigned int slot)
{
	int err = 0;
	struct ufs_hba *hba = container_of(ksm, struct ufs_hba, ksm);

	pm_runtime_get_sync(hba->dev);
	err = ufshcd_hold(hba, false);
	if (err) {
		pr_err("%s: failed to enable clocks, err %d\n", __func__, err);
		return err;
	}

	err = crypto_qti_keyslot_evict(hba->priv, slot);
	if (err) {
		pr_err("%s: failed with error %d\n",
			__func__, err);
		ufshcd_release(hba);
		pm_runtime_put_sync(hba->dev);
		return err;
	}

	ufshcd_release(hba);
	pm_runtime_put_sync(hba->dev);

	return err;
}

static int ufshcd_crypto_qti_derive_raw_secret(struct blk_keyslot_manager *ksm,
					       const u8 *wrapped_key,
					       unsigned int wrapped_key_size,
					       u8 *secret,
					       unsigned int secret_size)
{
	int err = 0;
	struct ufs_hba *hba = container_of(ksm, struct ufs_hba, ksm);

	pm_runtime_get_sync(hba->dev);
	err = ufshcd_hold(hba, false);
	if (err) {
		pr_err("%s: failed to enable clocks, err %d\n", __func__, err);
		return err;
	}

	err =  crypto_qti_derive_raw_secret(hba->priv,
				wrapped_key, wrapped_key_size,
				secret, secret_size);

	ufshcd_release(hba);
	pm_runtime_put_sync(hba->dev);

	return err;
}

static const struct blk_ksm_ll_ops ufshcd_crypto_qti_ksm_ops = {
	.keyslot_program	= ufshcd_crypto_qti_keyslot_program,
	.keyslot_evict		= ufshcd_crypto_qti_keyslot_evict,
	.derive_raw_secret	= ufshcd_crypto_qti_derive_raw_secret,
};

static enum blk_crypto_mode_num ufshcd_blk_crypto_qti_mode_num_for_alg_dusize(
					enum ufs_crypto_alg ufs_crypto_alg,
					enum ufs_crypto_key_size key_size)
{
	/*
	 * This is currently the only mode that UFS and blk-crypto both support.
	 */
	if (ufs_crypto_alg == UFS_CRYPTO_ALG_AES_XTS &&
		key_size == UFS_CRYPTO_KEY_SIZE_256)
		return BLK_ENCRYPTION_MODE_AES_256_XTS;

	return BLK_ENCRYPTION_MODE_INVALID;
}

static int ufshcd_hba_init_crypto_qti_spec(struct ufs_hba *hba)
{
	int cap_idx = 0;
	int err = 0;
	enum blk_crypto_mode_num blk_mode_num;

	/* Default to disabling crypto */
	hba->caps &= ~UFSHCD_CAP_CRYPTO;

	if (!(hba->capabilities & MASK_CRYPTO_SUPPORT)) {
		err = -ENODEV;
		goto out;
	}

	/*
	 * Crypto Capabilities should never be 0, because the
	 * config_array_ptr > 04h. So we use a 0 value to indicate that
	 * crypto init failed, and can't be enabled.
	 */
	hba->crypto_capabilities.reg_val =
			  cpu_to_le32(ufshcd_readl(hba, REG_UFS_CCAP));
	hba->crypto_cfg_register =
		 (u32)hba->crypto_capabilities.config_array_ptr * 0x100;
	hba->crypto_cap_array =
		 devm_kcalloc(hba->dev,
				hba->crypto_capabilities.num_crypto_cap,
				sizeof(hba->crypto_cap_array[0]),
				GFP_KERNEL);
	if (!hba->crypto_cap_array) {
		err = -ENOMEM;
		goto out;
	}

	/* The actual number of configurations supported is (CFGC+1) */
	err = blk_ksm_init(&hba->ksm, NUM_KEYSLOTS(hba));
	if (err)
		goto out;

	hba->ksm.ksm_ll_ops = ufshcd_crypto_qti_ksm_ops;
	/* UFS only supports 8 bytes for any DUN */
	hba->ksm.max_dun_bytes_supported = 8;
	hba->ksm.dev = hba->dev;

	/*
	 * Store all the capabilities now so that we don't need to repeatedly
	 * access the device each time we want to know its capabilities
	 */
	for (cap_idx = 0; cap_idx < hba->crypto_capabilities.num_crypto_cap;
	     cap_idx++) {
		hba->crypto_cap_array[cap_idx].reg_val =
				cpu_to_le32(ufshcd_readl(hba,
						REG_UFS_CRYPTOCAP +
						cap_idx * sizeof(__le32)));
		blk_mode_num = ufshcd_blk_crypto_qti_mode_num_for_alg_dusize(
				hba->crypto_cap_array[cap_idx].algorithm_id,
				hba->crypto_cap_array[cap_idx].key_size);
		if (blk_mode_num == BLK_ENCRYPTION_MODE_INVALID)
			continue;
		hba->ksm.crypto_modes_supported[blk_mode_num] |=
			hba->crypto_cap_array[cap_idx].sdus_mask * 512;
	}

	pr_debug("%s: keyslot manager created\n", __func__);

	return 0;

out:
	/* Indicate that init failed by setting crypto_capabilities to 0 */
	hba->crypto_capabilities.reg_val = 0;
	return err;
}

int ufshcd_crypto_qti_init_crypto(struct ufs_hba *hba,
				  const struct blk_ksm_ll_ops *ksm_ops)
{
	int err = 0;
	struct platform_device *pdev = to_platform_device(hba->dev);
	void __iomem *mmio_base;
	struct resource *mem_res;
	void __iomem *hwkm_ice_mmio = NULL;
	struct resource *hwkm_ice_memres = NULL;

	mem_res = platform_get_resource_byname(pdev, IORESOURCE_MEM,
								"ufs_ice");
	mmio_base = devm_ioremap_resource(hba->dev, mem_res);
	if (IS_ERR(mmio_base)) {
		pr_err("%s: Unable to get ufs_crypto mmio base\n", __func__);
		hba->caps &= ~UFSHCD_CAP_CRYPTO;
		hba->quirks |= UFSHCD_QUIRK_BROKEN_CRYPTO_ENABLE;
		return err;
	}

	hwkm_ice_memres = platform_get_resource_byname(pdev, IORESOURCE_MEM,
						       "ufs_ice_hwkm");

	if (!hwkm_ice_memres) {
		pr_err("%s: Either no entry in dtsi or no memory available for IORESOURCE\n",
		       __func__);
	} else {
		hwkm_ice_mmio = devm_ioremap_resource(hba->dev,
						      hwkm_ice_memres);

		if (IS_ERR(hwkm_ice_mmio)) {
			err = PTR_ERR(hwkm_ice_mmio);
			pr_err("%s: Error = %d mapping HWKM memory\n",
				__func__, err);
			return err;
		}
	}

	err = ufshcd_hba_init_crypto_qti_spec(hba);
	if (err) {
		pr_err("%s: Error initiating crypto capabilities, err %d\n",
					__func__, err);
		return err;
	}

	err = crypto_qti_init_crypto(hba->dev, mmio_base, hwkm_ice_mmio,
				     (void **)&hba->priv);
	if (err) {
		pr_err("%s: Error initiating crypto, err %d\n",
					__func__, err);
	}
	return err;
}

int ufshcd_crypto_qti_debug(struct ufs_hba *hba)
{
	return crypto_qti_debug(hba->priv);
}

int ufshcd_crypto_qti_resume(struct ufs_hba *hba,
			     enum ufs_pm_op pm_op)
{
	return crypto_qti_resume(hba->priv);
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("UFS Crypto ops QTI implementation");
