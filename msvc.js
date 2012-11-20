
/* this module builds using Visual Studio 10 and nmake (MSVC) */

var child_process = require('child_process')
,	util = require('util')
,	os = require('os')

	/* resolve path to visual studio */
,	MSVC_DIR_CMD = 'reg query "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\\Setup\\VC" /v ProductDir'
,	MSVC_DIR_PATTERN = /\s*ProductDir\s*REG_SZ\s*(.*)/g

	/* build helper script */
,	MSVC_BUILD_CMD = 'msvc-build-'+os.arch()+'.bat'
;

module.exports = function msvc(cb) {
	
	// setup environment...
	console.log("resolving MS Visual Studio 10...");
	child_process.exec(MSVC_DIR_CMD, {
		encoding: 'utf8',
		timeout: 10000,
		maxBuffer: 2*4096,
		killSignal: 'SIGTERM',
		cwd: __dirname,
		env: process.env
	}, function (err, stdout, stderr) {
		if (err) {
			console.log("Unable to query Visual Studio 10 installation folder from the Registry.");
			cb(err);
		} else {
			var match = MSVC_DIR_PATTERN.exec(stdout.toString());
			if (match) {
				process.env.PATH = match[1] + ';' + process.env.PATH;
				
				// perform build...
				console.log("performing MSVC build...");
				child_process.spawn(MSVC_BUILD_CMD, [], {
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
				
			} else {
				cb('Unable to find the Visual Studio installation directory in the Registry.');
			}
		}
	});
};
