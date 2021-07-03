/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup MEASUREMENT Measurements
 *
 * @brief SI-based measurement classification system
 * 
 * Measurements consist of a measurement type (Base Type + Extended Type),
 * represented in a specific SI unit (SI Unit Type), and implemented in a
 * specific C type in memory (C Type).
 *
 * There is an option to adjust the measurement's scale in +/- 10^n steps (Scale
 * Factor) from the default SI unit and scale indicated by the SI Unit Type.
 * For example, if 'Ampere' is indicated as the SI unit, the measurement could
 * indicate that the value is in uA by setting the scale factor to -6.
 * 
 * \ingroup MEASUREMENT
 *  @{
 */

#ifndef ZSL_MEASUREMENT_H__
#define ZSL_MEASUREMENT_H__

#include <zsl/zsl.h>
#include <zsl/measurement/base.h>
#include <zsl/measurement/ext_color.h>
#include <zsl/measurement/ext_light.h>
#include <zsl/measurement/ext_temperature.h>
#include <zsl/measurement/unit.h>

/**
 * @file measurement.h
 * @brief API header file for ZSL measurements.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Measurement header. All fields in little endian. */
struct zsl_mes_header {
	/** Filter (header upper word). */
	union {
		struct {
			/** Base measurement type. */
			uint8_t base_type;
			/** Extended measurement type (meaning depends on base type). */
			uint8_t ext_type;
			/** Flags */
			union {
				struct {
					/** Data format used (1 = CBOR). */
					uint16_t data_format : 3;
					/** Payload encoding used (1 = BASE64). */
					uint16_t encoding : 4;
					/** Compression algorithm used (1 = LZ4). */
					uint16_t compression : 3;
					/** Timestamp format (1 = epoch32, 2 = epoch64). */
					uint16_t timestamp : 3;
					/** Reserved for future use. Must be set to 0. */
					uint16_t _rsvd : 3;
				} flags;
				/** Flag bits (cbor, timestamp, etc.). */
				uint16_t flags_bits;
			};
		} filter;
		/** 32-bit representation of all Filter bits. */
		uint32_t filter_bits;
	};

	/** Unit (header middle word). */
	union {
		struct {
			/**
			 * @brief The SI unit and default scale used for this measurement.
			 * Must be a member of zsl_mes_unit_si.
			 */
			uint16_t si_unit;

			/**
			 * @brief The data type that this SI unit is represented by in C.
			 * Must be a member of zsl_mes_unit_ctype.
			 *
			 * This field can be used to determine how many bytes are required
			 * to represent this measurement value, and how to interpret the
			 * value in memory.
			 */
			uint8_t ctype;

			/**
			 * @brief The amount to scale the measurement value up or down
			 * (10^n), starting from the unit and scale indicated by si_unit.
			 * Typically, but not necessarily a member of zsl_mes_unit_scale.
			 */
			int8_t scale_factor;
		} unit;
		/** 32-bit representation of si_unit, ctype and scale_factor. */
		uint32_t unit_bits;
	};

	/** Src/Len (header lower word). */
	union {
		struct {
			/** Payload length, excluding the header, including timestamp. */
			uint16_t len;
			struct {
				/** Indicates this is a fragment of a larger packet. */
				uint8_t fragment : 2;
				/** Reserved for future use. Must be set to 0. */
				uint8_t _rsvd : 2;
				/** Sample count (2^n), 0 = 1 sample */
				uint8_t samples : 4;
			};
			/** Data source registery ID associated with this sample. */
			uint8_t sourceid;
		} srclen;
		/** 32-bit representation of all Src/Len bits. */
		uint32_t srclen_bits;
	};
};

/**
 * @brief Measurement packet wrapper.
 */
struct zsl_measurement {
	/** Packet header containing filter data, SI unit and payload length. */
	struct zsl_mes_header header;

	/** Payload contents. */
	void *payload;
};

/** Payload data structure used. */
enum zsl_mes_format {
	/** No data structure (raw C type data). */
	ZSL_MES_FORMAT_NONE     = 0,
	/** CBOR record(s). */
	ZSL_MES_FORMAT_CBOR     = 1,
};

/** Payload encoding used. */
enum zsl_mes_encoding {
	/** No encoding used (binary data). */
	ZSL_MES_ENCODING_NONE   = 0,
	/** BASE64 Encoding (rfc4648). */
	ZSL_MES_ENCODING_BASE64 = 1,
	/** BASE45 Encoding (draft-faltstrom-base45-06). */
	ZSL_MES_ENCODING_BASE45 = 2,
};

/** Payload compression algorithm used. */
enum zsl_mes_compression {
	/** No payload compression used. */
	ZSL_MES_COMPRESSION_NONE        = 0,
	/** LZ4 compression. */
	ZSL_MES_COMPRESSION_LZ4         = 1,
};

/** Packet fragments. */
enum zsl_mes_fragment {
	/** No a fragment (complete payload). */
	ZSL_MES_FRAGMENT_NONE           = 0,
	/** Non-final fragment in a larger payload. */
	ZSL_MES_FRAGMENT_PARTIAL        = 1,
	/** Final fragment in the larger payload. */
	ZSL_MES_FRAGMENT_FINAL          = 2,
};

/** Optional timestamp format used. */
enum zsl_mes_timestamp {
	/** No timestamp included. */
	ZSL_MES_TIMESTAMP_NONE          = 0,
	/** 32-bit Unix epoch timestamp present. */
	ZSL_MES_TIMESTAMP_EPOCH_32      = 1,
	/** 64-bit Unix epoch timestamp present. */
	ZSL_MES_TIMESTAMP_EPOCH_64      = 2,
	/** 32-bit millisecond device uptime counter. */
	ZSL_MES_TIMESTAMP_UPTIME_MS_32  = 3,
	/** 64-bit millisecond device uptime counter. */
	ZSL_MES_TIMESTAMP_UPTIME_MS_64  = 4,
	/** 64-bit microsecond device uptime counter. */
	ZSL_MES_TIMESTAMP_UPTIME_US_64  = 5,
};

/**
 * @brief Helper function to display the contents of the zsl_measurement.
 *
 * @param sample zsl_measurement to print.
 */
void zsl_mes_print(struct zsl_measurement *sample);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZSL_MEASUREMENT_H_ */
