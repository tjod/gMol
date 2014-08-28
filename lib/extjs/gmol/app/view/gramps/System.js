Ext.define('Gmol.view.gramps.System' ,{
    extend: 'Ext.Panel',
    alias : 'widget.grampssystem',
    title: 'system',
    autoScroll: true,
    initComponent: function() {
   		this.html = "Show/system output";
        this.callParent(arguments);
    }
});
