'use strict';

import fs from 'fs';
import gulp from 'gulp';
import gulpLoadPlugins from 'gulp-load-plugins';
import pjson from './package.json';
import minimist from 'minimist';
import wrench from 'wrench';

import {jsWatch} from './gulp/config/shared-vars';

// Load all gulp plugins based on their names
// EX: gulp-copy -> copy
const plugins = gulpLoadPlugins();

const defaultNotification = function (err) {
	return {
		subtitle: err.plugin,
		message: err.message,
		sound: 'Funk',
		onLast: true,
	};
};

let args = minimist(process.argv.slice(2));
let rootFolder;

// Use real cdpp folder if exists.
// @see README.md in repo root if not using the real cdpp github repo.
if (fs.existsSync('../cdpp')) {
	rootFolder = '../cdpp';
}
else if (fs.existsSync('../docroot/sites/default/themes/site/cdpp')) {
	rootFolder = '../docroot/sites/default/themes/site/cdpp';
}
else {
	console.log("ERROR: Theme folder cannot be found. \n" +
	"It shoud be in either: \n" +
	"- <repo root>/cdpp (if using or linked up with real cdpp github repo)\n" +
	"- <repo root>/docroot/sites/default/themes/site/cdpp (if not using cdpp github repo)\n");
	throw 'ERROR: Theme folder cannot be found.';
}

// This will grab all js in the `gulp` directory
// in order to load all gulp tasks
wrench.readdirSyncRecursive('./gulp/tasks').filter((file) => {
	return (/\.(js)$/i).test(file);
}).map(function (file) {
	require('./gulp/tasks/' + file)(gulp, plugins, args, rootFolder);
});

// Compiles all the code
gulp.task('compile', gulp.parallel(
	'sass',
));

// Default task
gulp.task('default', gulp.series(
	'compile',
	'watch',
));

