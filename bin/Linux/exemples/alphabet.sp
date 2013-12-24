# Fonction testant le PGCD

Func
[
	name = alphabet;

	Code
	[
		var[ type=char; name=lettre; value='a';]

		while ( lettre <= 'z')
		[
			out(lettre, ' ');
			lettre++;
		]
	]
]



# 'Main' du programme
start
[
	alphabet();
]
