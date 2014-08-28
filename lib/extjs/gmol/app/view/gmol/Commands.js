Ext.define('Gmol.view.Commands' ,{
    extend: 'Ext.form.Panel',
    alias : 'widget.commands',
    title: 'Commands',
    layout: 'anchor',
    style: 'color: blue',
    items: [
            {xtype: 'textfield', name: 'command',  anchor: '100%  5%',
            	minHeight:24, maxHeight:24, emptyText: 'enter commands'},
            {xtype: 'textarea',  name: 'output',   anchor: '100% 95%',
            	autoScroll: true, readOnly: true, fieldCls: 'ux-command', emptyText: 'command output'}
            ],
    initComponent: function() {
        this.callParent(arguments);
    }
});
