Ext.define('Gmol.view.gmol.System' ,{
    extend: 'Ext.Panel',
    alias : 'widget.gmolsystem',
    title: 'system',
    bodyPadding: 20,
    autoScroll: true,
    initComponent: function() {
   		this.html = "Show system output";
        this.callParent(arguments);
    }
});
