#!/usr/bin/env bash

#	----------------------------------------------------------------
# installer for openHAB configuration files
#	----------------------------------------------------------------


# set openHAB configuration directory
OPENHAB_CFG=/etc/openhab/configurations
# set openHAB user and group
OPENHAB_USER=openhab
OPENHAB_GROUP=openhab
# define openHAB file extensions
OPENHAB_FILE_EXTENSIONS=(items persist rules script sitemap map scale xsl)
# define openHAB directory names
# must be entry for each file extension
OPENHAB_CONF_DIRECTORIES=(items persistence rules scripts sitemaps transform transform transform)

PROGNAME=$(basename $0)

#	----------------------------------------------------------------
#	Function to print script start message
#	----------------------------------------------------------------
function start_message
{
	echo "Creating links to OpenHAB configuration files" 1>&2
  echo "Links will be created in directory ${OPENHAB_CFG}" 1>&2
}

#	----------------------------------------------------------------
#	Function for exit due to fatal program error
#		Accepts 1 argument:
#			string containing descriptive error message
#	----------------------------------------------------------------
function error_exit
{
	echo "${PROGNAME}: ${1:-"Unknown Error"}" 1>&2
	exit 1
}

#	----------------------------------------------------------------
#	Function to ensure script is run as sudo
#   Forces script to exit if not run as sudo
#	----------------------------------------------------------------
function ensure_sudo
{
  if (( $EUID != 0 )) ;
  then
    error_exit "Please run as root"
  fi
}

# ------------------------------------------------------------------
#	Function to create a symbolic link in a given directory
# to a given file in the current directory
#		Accepts 2 arguments:
#     File symbolic link points to
#     Directory symbolic link to be created in
#	----------------------------------------------------------------
function create_link
{
  DIR_=$2
  FILE_=$1
  if [ -d "$DIR_" ] ;
  then
    if [ ! -L "$DIR_/$FILE_" ] ;
    then
      ln -s ${PWD}/$FILE_ "$DIR_"
      STR_="Link created"
    else
      STR_="Link already exists"
    fi
  else
    STR_="Required directory $DIR_ does not exist"
  fi
  printf "File %s: %s\n" "$FILE_" "$STR_"
}

# ------------------------------------------------------------------
#	Function to set given user and group permissions
# for all files within a given directory
#		Accepts 3 arguments:
#     User
#     Group
#     Directory
#	----------------------------------------------------------------
function set_permissions
{
  USER_="$1"
  GROUP_="$2"
  DIR_="$3"
  chown -R "$USER_":"$GROUP_" "$DIR_"
}

# ------------------------------------------------------------------
#	Function to check if arrays OPENHAB_FILE_EXTENSIONS and
# OPENHAB_CONF_DIRECTORIES are the same length
#   Forces script to exit if array lengths are different
#	----------------------------------------------------------------
function check_array_lengths_match
{
  if [ ${#OPENHAB_FILE_EXTENSIONS[@]} -ne ${#OPENHAB_CONF_DIRECTORIES[@]} ] ;
  then
    error_exit "Length of OPENHAB_FILE_EXTENSIONS and OPENHAB_CONF_DIRECTORIES arrays must match"
  fi
}

# ------------------------------------------------------------------
#	Function process files of specified type in current directory
#	----------------------------------------------------------------
function link_files
{
  iter=0
  for FILETYPE in ${OPENHAB_FILE_EXTENSIONS[@]} ;
  do
    DIR=${OPENHAB_CONF_DIRECTORIES[iter]}
#    shopt -s nullglob
    for FILE in *.$FILETYPE ;
    do
      # check file has been found
      if [[ ! -e "$FILE" ]] ;
      then
        continue
      fi
      create_link "$FILE" "${OPENHAB_CFG}/$DIR"
    done
#    shopt -u nullglob
    iter=$(( $iter + 1 ))
  done
}


# ------------------------------------------------------------------
# main script execution starts here
# ------------------------------------------------------------------

# error checks
ensure_sudo
check_array_lengths_match

start_message

# process files in current directory if specified
# openHAB configuration directory exists
if [ -d ${OPENHAB_CFG} ] ;
then
  link_files
#  set_permissions "$OPENHAB_USER" "$OPENHAB_GROUP" "$OPENHAB_CFG"
  set_permissions "$OPENHAB_USER" "$OPENHAB_GROUP" "$OPENHAB_CFG"
fi

exit 0
