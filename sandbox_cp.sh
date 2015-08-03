#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

echo "Syncing Database from @sandbox to local..."
drush -y @sandbox sql-dump --result-file=/tmp/cdpp.sql
rsync $1@sandbox:/tmp/cdpp.sql /tmp/cdpp.sql
drush -y sql-drop
drush -y sql-cli < /tmp/cdpp.sql
echo "✔ Synced Database"

echo "Syncing Files..."
drush -y rsync @sandbox:%files/ @self:sites/default/files
echo "✔ Synced Files"

echo "Clearing Caches..."
drush cc all
echo "✔ Cached Cleared"

wget -qO- http://cdpp.dev/user &> /dev/null
echo "✔ All done."
