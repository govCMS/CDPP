'use strict';

export default function(gulp, plugins, args, rootFolder) {

  // Watch task
  gulp.task('watch', (done) => {

    // Styles
    gulp.watch([rootFolder, '**/*.{scss,sass}'].join('/'))
    .on('change', gulp.series('sass'));

    done();
  });
}
