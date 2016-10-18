#!/usr/bin/env bash

# installer for openHAB configuration files

# set openHAB configuration directory
OPENHAB_CFG=/etc/openhab/configurations
# set openHAB user and group
OPENHAB_USER=openhab
OPENHAB_GROUP=openhab


function ensure_sudo {
  if (( $EUID != 0 )); then
    echo "Please run as root"
    exit
  fi
}

function create_link {
  if [ -d "$2" ] ; then
    if [ ! -L "$2/$1" ] ; then
      ln -s ${PWD}/$1 "$2"
      echo "Link created for file $1"
    else
      echo "Link already exists for file $1"
    fi
  else
    echo "Directory $2 does not exist!"
  fi
}

function set_permissions {
  chown -R "$OPENHAB_USER":"$OPENHAB_USER" ${OPENHAB_CFG}
}


# script execution starts here

ensure_sudo

echo "Creating links to OpenHAB configuration files in directory ${OPENHAB_CFG} ..."

if [ -d ${OPENHAB_CFG} ] ; then
  for FILE in *.items ;
  do
    DIR="${OPENHAB_CFG}/items"
    create_link "$FILE" "$DIR"
  done
  for FILE in *.rules ;
  do
    DIR="${OPENHAB_CFG}/rules"
    create_link "$FILE" "$DIR"
  done
  for FILE in *.sitemap ;
  do
    DIR="${OPENHAB_CFG}/sitemaps"
    create_link "$FILE" "$DIR"
  done
  set_permissions
fi

exit
