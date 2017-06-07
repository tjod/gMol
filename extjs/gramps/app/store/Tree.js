Ext.define('Gramps.store.Tree', {
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
		gparse = function(node, index, t) {
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
		var gtree = Ext.decode(gramps.cmd("json/tree"));
		Ext.iterate(gtree.LWorld, gparse);
		
		var aroot = {
			text: 'LWorld',
			iconCls: 'group',
			expanded: true, 
			children: gtree.LWorld
		};
		return aroot;
	}
});