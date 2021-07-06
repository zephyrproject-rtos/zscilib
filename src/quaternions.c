/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <math.h>
#include <errno.h>
#include <zsl/zsl.h>
#include <zsl/quaternions.h>
#include <zsl/vectors.h>

/**
 * @brief Helper function to compare float values.
 *
 * @param a 		First float too compare.
 * @param b 		Second float to compare.
 * @param epsilon 	Allowed deviatin between first and second values.

 * @return true		If values are the same within the limits of epsilon.
 * @return false 	If values are different to an extent greater than epsilon.
 */
static bool
zsl_quat_val_is_equal(zsl_real_t a, zsl_real_t b, zsl_real_t epsilon)
{
	zsl_real_t c;

	c = a - b;

	if (c < epsilon && -c < epsilon) {
		return 1;
	} else {
		return 0;
	}
}

void zsl_quat_init(struct zsl_quat *q, enum zsl_quat_type type)
{
	switch (type) {
	case ZSL_QUAT_TYPE_IDENTITY:
		q->r = 1.0;
		q->i = 0.0;
		q->j = 0.0;
		q->k = 0.0;
		break;
	case ZSL_QUAT_TYPE_EMPTY:
	default:
		q->r = 0.0;
		q->i = 0.0;
		q->j = 0.0;
		q->k = 0.0;
		break;
	}
}

zsl_real_t zsl_quat_magn(struct zsl_quat *q)
{
	return ZSL_SQRT(q->r * q->r + q->i * q->i + q->j * q->j + q->k * q->k);
}

int zsl_quat_to_unit(struct zsl_quat *q, struct zsl_quat *qn)
{
	int rc = 0;
	zsl_real_t m = zsl_quat_magn(q);

	if ((m == 0.0) || (m == -0.0)) {
		qn->r = 0.0;
		qn->i = 0.0;
		qn->j = 0.0;
		qn->k = 0.0;
	} else {
		qn->r = q->r / m;
		qn->i = q->i / m;
		qn->j = q->j / m;
		qn->k = q->k / m;
	}

	return rc;
}

int zsl_quat_to_unit_d(struct zsl_quat *q)
{
	return zsl_quat_to_unit(q, q);
}

bool zsl_quat_is_unit(struct zsl_quat *q)
{
	zsl_real_t unit_len;

	/* Verify that sqrt(r^2+i^2+j^2+k^2) = 1.0. */
	unit_len = ZSL_SQRT(
		q->r * q->r +
		q->i * q->i +
		q->j * q->j +
		q->k * q->k);

	return zsl_quat_val_is_equal(unit_len, 1.0, 1E-6);
}

int zsl_quat_scale(struct zsl_quat *q, zsl_real_t s, struct zsl_quat *qs)
{
	int rc = 0;

	qs->r = q->r * s;
	qs->i = q->i * s;
	qs->j = q->j * s;
	qs->k = q->k * s;

	return rc;
}

int zsl_quat_scale_d(struct zsl_quat *q, zsl_real_t s)
{
	return zsl_quat_scale(q, s, q);
}

int zsl_quat_mult(struct zsl_quat *qa, struct zsl_quat *qb,
		  struct zsl_quat *qm)
{
	int rc = 0;

	qm->i = qa->r * qb->i + qa->i * qb->r + qa->j * qb->k - qa->k * qb->j;
	qm->j = qa->r * qb->j - qa->i * qb->k + qa->j * qb->r + qa->k * qb->i;
	qm->k = qa->r * qb->k + qa->i * qb->j - qa->j * qb->i + qa->k * qb->r;
	qm->r = qa->r * qb->r - qa->i * qb->i - qa->j * qb->j - qa->k * qb->k;

	return rc;
}

int zsl_quat_exp(struct zsl_quat *q, struct zsl_quat *qe)
{
	int rc = 0;

	ZSL_VECTOR_DEF(v, 3);
	zsl_real_t vmag;        /* Magnitude of v. */
	zsl_real_t vsin;        /* Sine of vm. */
	zsl_real_t rexp;        /* Exponent of q->r. */

	/* Populate the XYZ vector using ijk from q. */
	v.data[0] = q->i;
	v.data[1] = q->j;
	v.data[2] = q->k;

	/* Calculate magnitude of v. */
	vmag = zsl_vec_norm(&v);

	/* Normalise v to unit vector. */
	zsl_vec_to_unit(&v);

	vsin = ZSL_SIN(vmag);
	rexp = ZSL_EXP(q->r);

	qe->r = ZSL_COS(vmag) * rexp;
	qe->i = v.data[0] * vsin * rexp;
	qe->j = v.data[1] * vsin * rexp;
	qe->k = v.data[2] * vsin * rexp;

	return rc;
}

int zsl_quat_log(struct zsl_quat *q, struct zsl_quat *ql)
{
	int rc = 0;

	ZSL_VECTOR_DEF(v, 3);   /* Vector part of unit quat q. */
	zsl_real_t qmag;        /* Magnitude of q. */
	zsl_real_t racos;       /* Acos of q->r/qmag. */

	/* Populate the XYZ vector using ijk from q. */
	v.data[0] = q->i;
	v.data[1] = q->j;
	v.data[2] = q->k;

	/* Normalise v to unit vector. */
	zsl_vec_to_unit(&v);

	/* Calculate magnitude of input quat. */
	qmag = zsl_quat_magn(q);

	racos = ZSL_COS(q->r / qmag);

	ql->r = ZSL_LOG(qmag);
	ql->i = v.data[0] * racos;
	ql->j = v.data[1] * racos;
	ql->k = v.data[2] * racos;

	return rc;
}

int zsl_quat_pow(struct zsl_quat *q, zsl_real_t exp,
		 struct zsl_quat *qout)
{
	int rc = 0;

	struct zsl_quat qlog;
	struct zsl_quat qsc;

	rc = zsl_quat_log(q, &qlog);
	if (rc) {
		goto err;
	}

	rc = zsl_quat_scale(&qlog, exp, &qsc);
	if (rc) {
		goto err;
	}

	rc = zsl_quat_exp(&qsc, qout);
	if (rc) {
		zsl_quat_init(qout, ZSL_QUAT_TYPE_EMPTY);
		goto err;
	}

err:
	return rc;
}

int zsl_quat_conj(struct zsl_quat *q, struct zsl_quat *qc)
{
	int rc = 0;

	/* TODO: Check for div by zero before running this! */
	qc->r = q->r;
	qc->i = q->i * -1.0;
	qc->j = q->j * -1.0;
	qc->k = q->k * -1.0;

	return rc;
}

int zsl_quat_inv(struct zsl_quat *q, struct zsl_quat *qi)
{
	int rc = 0;
	zsl_real_t m = zsl_quat_magn(q);

	if ((m == 0.0) || (m == -0.0)) {
		/* Set to all 0's. */
		zsl_quat_init(qi, ZSL_QUAT_TYPE_EMPTY);
	} else {
		/* TODO: Check for div by zero before running this! */
		m *= m;
		qi->r = q->r / m;
		qi->i = q->i / -m;
		qi->j = q->j / -m;
		qi->k = q->k / -m;
	}

	return rc;
}

int zsl_quat_inv_d(struct zsl_quat *q)
{
	return zsl_quat_inv(q, q);
}

int zsl_quat_diff(struct zsl_quat *qa, struct zsl_quat *qb,
		  struct zsl_quat *qd)
{
	int rc;
	struct zsl_quat qi;

	rc = zsl_quat_inv(qa, &qi);
	if (rc) {
		goto err;
	}

	/* Note: order matters here!*/
	rc = zsl_quat_mult(&qi, qb, qd);

err:
	return rc;
}

int zsl_quat_slerp(struct zsl_quat *qa, struct zsl_quat *qb,
		   zsl_real_t t, struct zsl_quat *qi)
{
	int rc = 0;
	struct zsl_quat q1, q2;	/* Interim quats. */
	zsl_real_t dot;			/* qa->r * qb->r. */
	zsl_real_t phi;			/* arccos(dot). */
	zsl_real_t phi_s;		/* sin(phi). */
	zsl_real_t phi_st;		/* sin(phi * (t)). */
	zsl_real_t phi_smt;		/* sin(phi * (1.0 - t)). */
	
	/* 
	 * Unit quaternion slerp = qa * (qa^-1 * qb)^t
	 *
	 * We get there in a round-about way in this code, but we avoid pushing
	 * and popping values on the stack with trivial calls to helper functions.
	 */

	/* TODO: When t = 0.0 or t = 1.0, just memcpy qa or qb. */

	/* Make sure t is in a valid range. */
	t = t < 0.0 ? 0.0 : t;
	t = t > 1.0 ? 1.0 : t;

	dot = qa->r * qb->r;

	/*
	 * Slerp often has problems with angles close to zero. Consider handling
	 * those edge cases slightly differently?
	 */

	/* Calculate these once before-hand. */
	phi = ZSL_ACOS(dot);
	phi_s = ZSL_SIN(phi);
	phi_st = ZSL_SIN(phi * t);
	phi_smt = ZSL_SIN(phi * (1.0 - t));

	/* Calculate intermediate quats. */
	q1.r = phi_smt / phi_s * qa->r;
	q1.i = phi_smt / phi_s * qa->i;
	q1.j = phi_smt / phi_s * qa->j;
	q1.k = phi_smt / phi_s * qa->k;	
	q2.r = phi_st / phi_s * qb->r;
	q2.i = phi_st / phi_s * qb->i;
	q2.j = phi_st / phi_s * qb->j;
	q2.k = phi_st / phi_s * qb->k;

	/* Final result = q1 + q2. */
	qi->r = q1.r + q2.r;
	qi->i = q1.i + q2.i;
	qi->j = q1.j + q2.j;
	qi->k = q1.k + q2.k;

	return rc;
}
