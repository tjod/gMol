Ext.define('Help.store.Topics', {
	extend: 'Ext.data.Store',
    fields: ['name','link'],
	data : [
		{link:"introduction.html", name:"Introduction"},
		{link:"command_syntax.html", name:"Command syntax"},
		{link:"symbols.html", name:"Symbols"},
		{link:"picture_files.html", name:"Picture files"},
		{link:"transformations.html", name:"Transformations"},
		{link:"function.html", name:"Function"},
		{link:"worlds.html", name:"Worlds"},
		{link:"macro_files.html", name:"Macros"}
	]
});