Ext.define('Gmol.store.gramps.Tree', {
	extend: 'Ext.data.TreeStore',
/*	root:  {
		text: 'Root',
		expanded: true, 
		children: [
		           {text: "nodea", leaf: true},
		           {text: "nodeb", leaf: true}
		           ]
	},*/
	parseTree: function () {
		// to get gramps json/tree and format for TreePanel
        var gparse = function(node, index, t) {
		    node.text = node.name;
		    if (node.children) {
		    	node.iconCls = 'group';
		    	node.cls = 'folder';
		    	Ext.iterate(node.children, gparse);
		    } else {
		    	node.iconCls = 'object';
		    	node.cls = 'file';
		    	node.leaf = true;
		    }
		};
		var gtree = gramps.cmd("json/tree");
		//console.log(gtree);
		//var jtree = Ext.decode(gtree);
		//console.log(jtree);
		//console.log(JSON.parse(gtree));
		var jtree = JSON.parse(gtree);
		Ext.iterate(jtree.LWorld, gparse);
		
		var aroot = {
			text: 'LWorld',
			iconCls: 'group',
			expanded: true, 
			children: jtree.LWorld
		};
		return aroot;
	}
});
