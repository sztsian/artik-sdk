/*
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 */

#ifndef __ARTIK_LOG_H
#define __ARTIK_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_log.h
	 *
	 *  \brief LOG module definition
	 *
	 *  Definitions and functions for accessing
	 *  the LOG module and select log level
	 */

	/*!
	 *  \brief logging levels.
	 */

	enum artik_log_level {
		/* Error level */
		LOG_LEVEL_ERROR,
		/* Warning level. Hide in RELEASE mode */
		LOG_LEVEL_WARNING,
		/* Information level. Hide in RELEASE mode */
		LOG_LEVEL_INFO,
		/* Debug level. Hide in RELEASE mode */
		LOG_LEVEL_DEBUG
	};

	/*!
	 * \brief     logging backend system
	 */
	enum artik_log_system {
		 /**< Standard error */
		LOG_SYSTEM_STDERR,
		/**< syslog */
		LOG_SYSTEM_SYSLOG,
		/**< no log */
		LOG_SYSTEM_NONE,
		/**< use custom log handler */
		LOG_SYSTEM_CUSTOM
	};

	/*!
	 * \brief     logging prefix
	 *
	 * Additional information to log message.
	 * Masking the field by log_set_prefix_fields()
	 */
	enum artik_log_prefix {
		/**< No prefix */
		LOG_PREFIX_NONE = 0,
		/**< mm-dd HH:MM:SS.000 */
		LOG_PREFIX_TIMESTAMP = (1 << 0),
		/**< Process ID */
		LOG_PREFIX_PID = (1 << 1),
		/**< Thread ID */
		LOG_PREFIX_TID = (1 << 2),
		/**< D, I, W, E */
		LOG_PREFIX_LEVEL = (1 << 3),
		/**< Full path with file name */
		LOG_PREFIX_FILEPATH = (1 << 4),
		/**< File name */
		LOG_PREFIX_FILENAME = (1 << 5),
		/**< Function name */
		LOG_PREFIX_FUNCTION = (1 << 6),
		/**< Line number */
		LOG_PREFIX_LINE = (1 << 7),
		/**< TIMESTAMP + PID + TID + LEVEL
		 * + FILENAME + LINE
		 */
		LOG_PREFIX_DEFAULT =
			(LOG_PREFIX_TIMESTAMP |
			LOG_PREFIX_PID |
			LOG_PREFIX_TID |
			LOG_PREFIX_LEVEL |
			LOG_PREFIX_FILENAME |
			LOG_PREFIX_LINE),
		/**< All prefix */
		LOG_PREFIX_ALL =
			(LOG_PREFIX_DEFAULT |
			LOG_PREFIX_FUNCTION |
			LOG_PREFIX_FILEPATH)
	};

	/*!
	 * \brief     Custom log hook handler
	 *
	 * Grab all log and deal own way. (e.g. custom file writing)
	 *
	 * \param[in] level log level
	 * \param[in] prefix generated additional information
	 *                   (e.g. timestamp, line number)
	 * \param[in] msg original log message
	 * \param[in] user_data The user data passed from the callback function
	 */
	typedef void (*artik_log_handler) (enum artik_log_level level,
					   const char *prefix, const char *msg,
					   void *user_data);

	/*! \struct artik_log_module
	 *
	 *  \brief Logging module operations
	 *
	 *  Structure containing all the operations exposed
	 *  by the logging module.
	 */
	typedef struct {
		/*!
		 * \brief     Set logging backend system
		 *
		 * \param[in] system Log system
		 *
		 * \return S_OK on success, error code otherwise
		 */
		artik_error(*set_system) (enum artik_log_system system);

		/*!
		 * \brief     Set custom log handler
		 *
		 * When set the handler, log system changed to LOG_SYSTEM_CUSTOM
		 *
		 * \param[in] handler Callback
		 * \param[in] user_data The user data to be passed to
		 *            the callback function
		 *
		 * \return S_OK on success, error code otherwise
		 */
		artik_error(*set_handler) (artik_log_handler handler,
					   void *user_data);

		/*!
		 * \brief     Set the additional information
		 *
		 * \param[in] field_set Bitmask by enum log_prefix
		 */
		void (*set_prefix_fields)(enum artik_log_prefix field_set);

		/*!
		 * \brief     Get the additional information
		 */
		enum artik_log_prefix (*get_prefix_fields)(void);

		/*!
		 * \brief     logging function
		 *
		 * Use convenient macro(e.g. log_dbg(),log_warn(), ...) instead
		 * of this api due to difficult to fill each parameters.
		 *
		 * \param[in] level Log level
		 * \param[in] filename Source file name (e.g. __FILE__)
		 * \param[in] funcname Function name (e.g. __FUNCTION__)
		 * \param[in] line Source file line number
		 * \param[in] format Printf format string
		 */
		void (*print)(enum artik_log_level level, const char *filename,
			       const char *funcname, int line,
			       const char *format, ...);

	} artik_log_module;

	extern const artik_log_module log_module;

	/*!
	 * \brief     Convenient macro to fill file, function
	 *            and line informations
	 * \param[in] level Logging level
	 * \param[in] fmt Printf format string
	 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#define artik_log(level, ...) (log_module.print(level, __FILE__, \
				__func__, __LINE__, __VA_ARGS__))

#ifdef CONFIG_RELEASE
#define log_dbg(...)
#define log_info(...)
#define log_warn(...)
#else
#define log_dbg(...) artik_log(LOG_LEVEL_DEBUG, __VA_ARGS__)
#define log_info(...) artik_log(LOG_LEVEL_INFO,  __VA_ARGS__)
#define log_warn(...) artik_log(LOG_LEVEL_WARNING, __VA_ARGS__)
#endif

#define log_err(...) artik_log(LOG_LEVEL_ERROR, __VA_ARGS__)

#pragma GCC diagnostic pop
#ifdef __cplusplus
}
#endif
#endif
