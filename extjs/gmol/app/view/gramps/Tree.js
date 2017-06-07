Ext.define('Gmol.view.gramps.Tree' ,{
    extend: 'Ext.tree.Panel',
    store: 'gramps.Tree',
    alias : 'widget.grampstree',
    title: 'tree',
    autoScroll: true,
    rootVisible: true,
    
    initComponent: function() {
        this.callParent(arguments);
    },

    refresh: function(opt) {
        var newroot = this.getStore().parseTree();
        this.setRootNode(newroot);
        this.expandAll();
    }
});
