var gulp = require('gulp');
var concat = require('gulp-concat');
var rename = require('gulp-rename');
var uglify = require('gulp-uglify');

//script paths
var js405 = [
    'exploits/vtx-405/fix.js',
    'exploits/vtx-405/syscalls.js',
    'exploits/vtx-405/index.js',
    'exploits/vtx-405/expl.js',
    'exploits/vtx-405/gadgets.js',
    'exploits/vtx-405/rop.js',
  ],
  output405 = 'exploit_4.05.js',
  output455 = 'exploit_4.55.js',
  jsDest = './public';

gulp.task('vtx-405', function() {
  return gulp
    .src(js405, { base: 'exploits/vtx-405/' })
    .pipe(concat(output405))
    .pipe(uglify())
    .pipe(gulp.dest(jsDest));
});
