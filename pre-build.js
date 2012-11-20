
/* downloads and unpacks a curses library then builds a static lib */

var fs = require('fs')
,	os = require('os')
,	tar = require('tar')
,	url = require('url')
,	path = require('path')
,	http = require('http')
,	zlib = require('zlib')

/* the operating system ['win32'|'linux'] */
,	OS_PLATFORM = os.platform()

/* the URL of the sources to build */
,	SOURCE_URL = require('./source-urls')[OS_PLATFORM]

/* the source directory without the ./deps/ prefix */
,	TARGET_DIR = path.basename(url.parse(SOURCE_URL).pathname, '.tar.gz')

/* show a fancy download progress bar*/
,	PROGRESS_BAR =	'0%       50%       100%\n'+
					'| . . . . | . . . . |'
					
/* extracts the contents of the tarball stream to ./deps/[TARGET_DIR] */
,	tarExtract = tar.Extract(path.resolve(__dirname, 'deps'))
	
/* unzips the gzip stream */
,	gzExtract = zlib.createUnzip()

/* load the module that performs the build in a platform specific manner */
,	buildTask = require( (OS_PLATFORM==='win32') ? './msvc' : './gcc' )

;

/* error out on existing deps folder... */
if (fs.existsSync(path.resolve(__dirname, 'deps'))) {
	console.log("a ./deps folder was found, stopping download so nothing gets overwritten");
	process.exit(1);
}

/* DOWNLOAD DEPENDENCIES ... */
console.log("download and unpack '%s' to folder '%s'...", SOURCE_URL, path.resolve(__dirname, 'deps', TARGET_DIR));
http
.	get(SOURCE_URL, function (res) {
		
		var length = Number(res.headers['content-length'])
		,	bytesRead = 0
		,	percent = 0
		,	diff = 0
		;
		
		console.log("filesize: %s bytes", length);
		console.log("\n%s", PROGRESS_BAR);
		process.stdout.write('#');
		res
		.	on('data', function (chunk) {
				
				bytesRead += chunk.length;
				var newPercent = Math.round(bytesRead/length*100);
				
				if (percent != newPercent) {
					diff += newPercent-percent;
					percent = newPercent;
					
					while (diff >= 5) {
						process.stdout.write('#');
						diff -= 5;
					}
					
				}
				
			})
		.	pipe(gzExtract)
		.	pipe(tarExtract)
		.	on('end', function () {
				
				console.log("\ndownload successful");
				
				/* BUILD DEPENDENCIES ... */
				buildTask(function (err) {
					if (err) {
						console.log("ERROR: %s", err);
						process.exit(1);
					} else {
						console.log("sucessfully built dependencies");
						process.exit(0);
					}
				});
				
			})
		;
		
	})
.	on('error', function (err) {
		
		console.log("\ndownload failed: %s", err);
		process.exit(1);
		
	})
;
