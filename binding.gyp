{
	'target_defaults' : {
		'default_configuration' : 'Release',
		'configurations' : {
			'Debug' : {
				'defines' : ['DEBUG', '_DEBUG'],
			},
			'Release' : {
				'defines' : ['NDEBUG'],
			}
		},
		'msvs_settings' : {
			'VCLinkerTool' : {
				'GenerateDebugInformation' : 'true',
			},
		},
		'conditions' : [
			[
				'OS=="win"',
				{
					'defines' : ['PDC_WIDE'],
					'include_dirs' : [
						'deps/PDCurses-3.4'
					],
					'libraries' : [
						'../deps/PDCurses-3.4/win32/pdcurses.lib',
						'../deps/PDCurses-3.4/win32/panel.lib'
					]
				}
			],
			[
				'OS!="win"',
				{
					'include_dirs' : [
						'deps/ncurses-5.7/include'
					],
					'ldflags' : [
						'-L../deps/ncurses-5.7/lib'
					],
					'libraries' : [
						'-lncursestw',
						'-lpaneltw'
					]
				}
			]
		],
	},
	
	'targets' : [
		{
			'target_name': 'curses',
			'sources': [
				'src/curses.cc'
			]
		}
	]
}
