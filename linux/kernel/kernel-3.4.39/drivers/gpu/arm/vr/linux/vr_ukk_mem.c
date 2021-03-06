/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from NEXELL Limited
 * (C) COPYRIGHT 2008-2013 NEXELL Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from NEXELL Limited.
 */
#include <linux/fs.h>       /* file system operations */
#include <asm/uaccess.h>    /* user space access */

#include "vr_ukk.h"
#include "vr_osk.h"
#include "vr_kernel_common.h"
#include "vr_session.h"
#include "vr_ukk_wrappers.h"

int mem_write_safe_wrapper(struct vr_session_data *session_data, _vr_uk_mem_write_safe_s __user * uargs)
{
	_vr_uk_mem_write_safe_s kargs;
	_vr_osk_errcode_t err;

	VR_CHECK_NON_NULL(uargs, -EINVAL);
	VR_CHECK_NON_NULL(session_data, -EINVAL);

	if (0 != copy_from_user(&kargs, uargs, sizeof(_vr_uk_mem_write_safe_s))) {
		return -EFAULT;
	}

	kargs.ctx = session_data;

	/* Check if we can access the buffers */
	if (!access_ok(VERIFY_WRITE, kargs.dest, kargs.size)
	    || !access_ok(VERIFY_READ, kargs.src, kargs.size)) {
		return -EINVAL;
	}

	/* Check if size wraps */
	if ((kargs.size + kargs.dest) <= kargs.dest
	    || (kargs.size + kargs.src) <= kargs.src) {
		return -EINVAL;
	}

	err = _vr_ukk_mem_write_safe(&kargs);
	if (_VR_OSK_ERR_OK != err) {
		return map_errcode(err);
	}

	if (0 != put_user(kargs.size, &uargs->size)) {
		return -EFAULT;
	}

	return 0;
}

int mem_map_ext_wrapper(struct vr_session_data *session_data, _vr_uk_map_external_mem_s __user * argument)
{
	_vr_uk_map_external_mem_s uk_args;
	_vr_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	VR_CHECK_NON_NULL( argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if ( 0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_vr_uk_map_external_mem_s)) ) {
		return -EFAULT;
	}

	uk_args.ctx = session_data;
	err_code = _vr_ukk_map_external_mem( &uk_args );

	if (0 != put_user(uk_args.cookie, &argument->cookie)) {
		if (_VR_OSK_ERR_OK == err_code) {
			/* Rollback */
			_vr_uk_unmap_external_mem_s uk_args_unmap;

			uk_args_unmap.ctx = session_data;
			uk_args_unmap.cookie = uk_args.cookie;
			err_code = _vr_ukk_unmap_external_mem( &uk_args_unmap );
			if (_VR_OSK_ERR_OK != err_code) {
				VR_DEBUG_PRINT(4, ("reverting _vr_ukk_unmap_external_mem, as a result of failing put_user(), failed\n"));
			}
		}
		return -EFAULT;
	}

	/* Return the error that _vr_ukk_free_big_block produced */
	return map_errcode(err_code);
}

int mem_unmap_ext_wrapper(struct vr_session_data *session_data, _vr_uk_unmap_external_mem_s __user * argument)
{
	_vr_uk_unmap_external_mem_s uk_args;
	_vr_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	VR_CHECK_NON_NULL( argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if ( 0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_vr_uk_unmap_external_mem_s)) ) {
		return -EFAULT;
	}

	uk_args.ctx = session_data;
	err_code = _vr_ukk_unmap_external_mem( &uk_args );

	/* Return the error that _vr_ukk_free_big_block produced */
	return map_errcode(err_code);
}

#if defined(CONFIG_VR400_UMP)
int mem_release_ump_wrapper(struct vr_session_data *session_data, _vr_uk_release_ump_mem_s __user * argument)
{
	_vr_uk_release_ump_mem_s uk_args;
	_vr_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	VR_CHECK_NON_NULL( argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if ( 0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_vr_uk_release_ump_mem_s)) ) {
		return -EFAULT;
	}

	uk_args.ctx = session_data;
	err_code = _vr_ukk_release_ump_mem( &uk_args );

	/* Return the error that _vr_ukk_free_big_block produced */
	return map_errcode(err_code);
}

int mem_attach_ump_wrapper(struct vr_session_data *session_data, _vr_uk_attach_ump_mem_s __user * argument)
{
	_vr_uk_attach_ump_mem_s uk_args;
	_vr_osk_errcode_t err_code;

	/* validate input */
	/* the session_data pointer was validated by caller */
	VR_CHECK_NON_NULL( argument, -EINVAL);

	/* get call arguments from user space. copy_from_user returns how many bytes which where NOT copied */
	if ( 0 != copy_from_user(&uk_args, (void __user *)argument, sizeof(_vr_uk_attach_ump_mem_s)) ) {
		return -EFAULT;
	}

	uk_args.ctx = session_data;
	err_code = _vr_ukk_attach_ump_mem( &uk_args );

	if (0 != put_user(uk_args.cookie, &argument->cookie)) {
		if (_VR_OSK_ERR_OK == err_code) {
			/* Rollback */
			_vr_uk_release_ump_mem_s uk_args_unmap;

			uk_args_unmap.ctx = session_data;
			uk_args_unmap.cookie = uk_args.cookie;
			err_code = _vr_ukk_release_ump_mem( &uk_args_unmap );
			if (_VR_OSK_ERR_OK != err_code) {
				VR_DEBUG_PRINT(4, ("reverting _vr_ukk_attach_mem, as a result of failing put_user(), failed\n"));
			}
		}
		return -EFAULT;
	}

	/* Return the error that _vr_ukk_map_external_ump_mem produced */
	return map_errcode(err_code);
}
#endif /* CONFIG_VR400_UMP */

int mem_query_mmu_page_table_dump_size_wrapper(struct vr_session_data *session_data, _vr_uk_query_mmu_page_table_dump_size_s __user * uargs)
{
	_vr_uk_query_mmu_page_table_dump_size_s kargs;
	_vr_osk_errcode_t err;

	VR_CHECK_NON_NULL(uargs, -EINVAL);
	VR_CHECK_NON_NULL(session_data, -EINVAL);

	kargs.ctx = session_data;

	err = _vr_ukk_query_mmu_page_table_dump_size(&kargs);
	if (_VR_OSK_ERR_OK != err) return map_errcode(err);

	if (0 != put_user(kargs.size, &uargs->size)) return -EFAULT;

	return 0;
}

int mem_dump_mmu_page_table_wrapper(struct vr_session_data *session_data, _vr_uk_dump_mmu_page_table_s __user * uargs)
{
	_vr_uk_dump_mmu_page_table_s kargs;
	_vr_osk_errcode_t err;
	void *buffer;
	int rc = -EFAULT;

	/* validate input */
	VR_CHECK_NON_NULL(uargs, -EINVAL);
	/* the session_data pointer was validated by caller */

	kargs.buffer = NULL;

	/* get location of user buffer */
	if (0 != get_user(buffer, &uargs->buffer)) goto err_exit;
	/* get size of mmu page table info buffer from user space */
	if ( 0 != get_user(kargs.size, &uargs->size) ) goto err_exit;
	/* verify we can access the whole of the user buffer */
	if (!access_ok(VERIFY_WRITE, buffer, kargs.size)) goto err_exit;

	/* allocate temporary buffer (kernel side) to store mmu page table info */
	VR_CHECK(kargs.size > 0, -ENOMEM);
	kargs.buffer = _vr_osk_valloc(kargs.size);
	if (NULL == kargs.buffer) {
		rc = -ENOMEM;
		goto err_exit;
	}

	kargs.ctx = session_data;
	err = _vr_ukk_dump_mmu_page_table(&kargs);
	if (_VR_OSK_ERR_OK != err) {
		rc = map_errcode(err);
		goto err_exit;
	}

	/* copy mmu page table info back to user space and update pointers */
	if (0 != copy_to_user(uargs->buffer, kargs.buffer, kargs.size) ) goto err_exit;
	if (0 != put_user((kargs.register_writes - (u32 *)kargs.buffer) + (u32 *)uargs->buffer, &uargs->register_writes)) goto err_exit;
	if (0 != put_user((kargs.page_table_dump - (u32 *)kargs.buffer) + (u32 *)uargs->buffer, &uargs->page_table_dump)) goto err_exit;
	if (0 != put_user(kargs.register_writes_size, &uargs->register_writes_size)) goto err_exit;
	if (0 != put_user(kargs.page_table_dump_size, &uargs->page_table_dump_size)) goto err_exit;
	rc = 0;

err_exit:
	if (kargs.buffer) _vr_osk_vfree(kargs.buffer);
	return rc;
}
