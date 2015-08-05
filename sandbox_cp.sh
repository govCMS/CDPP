#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [[ $# -eq 0 ]] ; then
    echo "✘ Try again with your username as first argument"
    exit 0
fi

echo "Syncing Database from @sandbox to local..."
drush -y @sandbox sql-dump --result-file=/tmp/cdpp.sql && rsync $1@sandbox:/tmp/cdpp.sql /tmp/cdpp.sql && drush -y sql-drop && drush -y sql-cli < /tmp/cdpp.sql && echo "✔ Synced Database" || echo "✘ An Error Occured."
echo ""

echo "Syncing Files..."
drush -y rsync @sandbox:%files/ @self:sites/default/files && echo "✔ Synced Files" || echo "✘ An Error Occured."
echo ""

echo "Clearing Caches..."
drush cc all && echo "✔ Cached Cleared"
echo ""

echo "Repopulating Cache..."
wget -qO- http://cdpp.dev/user &> /dev/null && wget -qO- http://cdpp.dev/ &> /dev/null && wget -qO- http://cdpp.local/user &> /dev/null && wget -qO- http://cdpp.local/ &> /dev/null && echo "✔ Cached Generated" || echo "✘ An Error Occured."
echo ""

echo "Doing theme git dry run..."
git fetch -v --dry-run && echo "✔ Up To Date" || echo "✘ An Error Occured."
echo ""

echo "✔ All done."
