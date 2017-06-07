Ext.define('Gramps.controller.Tree', {
    extend: 'Ext.app.Controller',
    stores: ['Tree'],
    //models: ['Tree'],
    views: ['Tree'],
    init: function() {
		this.control({
            //'viewport > tabpanel > panel[title="Tree"]': {
			showtree: {
                render: this.onPanelRendered,
                show: this.onPanelShown,
                expand: this.onPanelShown  // when accordion expands, not just shown
            }
        });
    },

    onPanelRendered: function(tree, opt) {
        Ext.log(tree.title + ' panel was rendered');
    },
    
    onPanelShown: function(tree, opt) {
        Ext.log(tree.title + ' panel was shown');
        var newroot = tree.getStore().parseTree();
   		tree.setRootNode(newroot);
   		tree.expandAll();
    }

});