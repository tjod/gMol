Ext.define('Gramps.view.System' ,{
    extend: 'Ext.Panel',
    alias : 'widget.showsystem',
    title: 'System',
    autoScroll: true,
    initComponent: function() {
   		this.html = "Show/system output";
        this.callParent(arguments);
    }
});