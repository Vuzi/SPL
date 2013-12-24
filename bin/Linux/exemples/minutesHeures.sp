# Fonction calculant les heures et minutes restantes

Func
[
	name = heureMinute;
	type = void;

	Args
	[
		Ref[ name = h; type = int; ]
		Ref[ name = m; type = int; ]
	]

	Code
	[
		var [ name = ret; type = int; value = 1;]

		h += m / 60;
		m = m % 60;
	]
]



# 'Main' du programme
start
[
	Var[ name = h; type = int; ]
	Var[ name = m; type = int; ]

	in(h, m);

	heureMinute(h,m);

	out(h, 'h', m, 'm');
]
