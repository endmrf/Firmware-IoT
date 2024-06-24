#! WARNING: This file was automatically generated at 2019-12-12 13:09:46 (UTC).
#!          Any changes to this file will be overwritten!
#! 


# The name of the project component
NAME := app_FirmwareDIM


$(NAME)_INCLUDES := .

# Enable/disable automatic generation of static function prototypes in project source code
$(NAME)_AUTO_PROTOTYPE := 1

# List of files to include in the project build. Paths relative to the project's directory
$(NAME)_SOURCES := button.c \
                   commands.c \
                   connection.c \
                   get_type.c \
                   json_parser.c \
                   main.c \
                   mqtt_cli_commands.c \
                   mqtt_cli_variables.c \
                   nvm_defaults.c \
                   nvm_defaults_validate.c

# Paths to app settings .ini files  (paths are relative to project directory)
$(NAME)_SETTINGS_INCLUDES := resources/settings.ini \
                             resources/network_tls.ini
# Include the webapp resource files
$(NAME)_REQUIRED_COMPONENTS := components.gecko_os.cloud.mqtt \
                               resources.gecko_os.webapp
                               
$(NAME)_RESOURCE_MANIFEST_PATH := resources/manifest.cfg

# The ID of the corresponding Eclipse project
ECLIPSE_PROJECT_ID := 735121494

# The hardware platform for which to build
BUILD_PLATFORM := wgm160p

# The corresponding DMS product for this project
DMS_PRODUCT_CODE := 0S07DX-WDIM01

# The type of build to perform
BUILD_TYPE := release

# If set, the NVM will not be modified after programming the device. This is useful for debugging so settings like SSID/password do not need to be set after every programming
PRESERVE_NVM := 0
