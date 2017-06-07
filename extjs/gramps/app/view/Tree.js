Ext.define('Gramps.view.Tree' ,{
    extend: 'Ext.tree.Panel',
    store: 'Tree',
    alias : 'widget.showtree',
    title: 'Tree',
    autoScroll: true,
    rootVisible: true,
    
    initComponent: function() {
        this.callParent(arguments);
    }
});