Ext.define('Gmol.store.gmol.Tree', {
	extend: 'Ext.data.TreeStore',
	model: 'Gmol.model.gmol.Tree',
	root: {
        expanded: false, 
        rowname: "Molecules",
        children: [],
        iconCls: 'group'
    },
	parseTree: function () {
		// to get gmol tree and format for TreePanel
        var gparse = function(node, index, t) {
			// add the appropriate items to allow folders/leafs in tree
		    //node.text = node.rowname;
			node.checked = (node.ignore == false);
		    if (node.children && node.children.length > 0) {
		    	node.iconCls = 'group';
		    	node.cls     = 'folder';
		    	Ext.iterate(node.children, gparse);
		    } else {
		    	node.iconCls = 'object';
		    	node.cls  = 'file';
		    	node.leaf = true;
		    }
		};
		var jtree = gmol.getTree(); // returns a Qt-owned object
		// make a JS-owned copy to mod in iterate
		//var gtree = Ext.decode(Ext.encode(jtree));
		var gtree = JSON.parse(JSON.stringify(jtree));
		Ext.iterate(gtree, gparse);
        //console.debug(gtree);
		
		var aroot = {
			rowname: 'Molecules',
			iconCls: 'group',
			expanded: true, 
			children: gtree
		};
		return aroot;
    }
});
