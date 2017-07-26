'use strict';

import path from 'path';
import autoprefixer from 'autoprefixer';
import px2rem from 'postcss-pxtorem';
import gulpif from 'gulp-if';
import notifier from 'node-notifier';
import { notification_icon_location } from '../config/shared-vars';

export default function(gulp, plugins, args, rootFolder) {

  const px2rem_settings = {
    rootValue: 10,
    propWhiteList: ['font', 'font-size', 'letter-spacing'],
    replace: false,
  };

  // Sass compilation
  gulp.task('sass', () => {
    return gulp.src(path.join(rootFolder, 'sass/style.scss'))
      .pipe(plugins.plumber((error)=>{
        console.log(`\n ${plugins.util.colors.red.bold('Sass failed to compile:')} ${plugins.util.colors.yellow(error.message)}\n`);
        //console.error(error.stack);
        return notifier.notify({title: 'Sass Error', message: `${path.basename(error.file)} line ${error.line}`, icon: notification_icon_location+'gulp-error.png'});
      }))
      .pipe(plugins.sourcemaps.init())
      .pipe(plugins.sass({
        outputStyle: 'nested',
        precision: 10,
        includePaths: [
          path.join(rootFolder, 'sass'),
          path.join('node_modules')
        ]
      }).on('error', plugins.sass.logError))
      .pipe(plugins.postcss([
        autoprefixer({browsers: ['last 2 version', '> 1%']}),
        px2rem(px2rem_settings)
      ]))
      .pipe(gulpif(args.production, plugins.cssnano({rebase: false})))
      .pipe(plugins.sourcemaps.write('./'))
      .pipe(gulp.dest([rootFolder, 'css'].join('/')))
  });
}
