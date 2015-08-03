#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo "Syncing Database from @sandbox to local"
drush -y @sandbox sql-dump --result-file=/tmp/cdpp.sql
rsync $1@sandbox:/tmp/cdpp.sql /tmp/cdpp.sql
drush -y sql-drop
drush -y sql-cli < /tmp/cdpp.sql
echo "Synced Database"
echo "Syncing Files"
drush -y rsync @sandbox:%files/ @self:sites/default/files
echo "Synced Files"

