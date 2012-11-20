
/* this module builds using configure and make (GCC) */

var child_process = require('child_process')
,	util = require('util')
,	os = require('os')

	/* build helper script */
,	GCC_BUILD_CMD = 'gcc-build-'+os.arch()+'.sh'
;

module.exports = function gcc(cb) {
	
	// perform build...
	console.log("performing GCC build...");
	child_process.spawn(process.env.SHELL, [GCC_BUILD_CMD], {
		cwd: __dirname,
		env: process.env,
		stdio: 'inherit'
	})
	.	on('exit', function (code, signal) {
			if (code != 0) {
				cb(util.format('build failed (%s)', code));
			} else {
				cb(null);
			}
		})
	;
	
};
