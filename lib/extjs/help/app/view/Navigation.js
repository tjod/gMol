Ext.define('Help.view.Navigation' ,{
    extend: 'Ext.panel.Panel',
    alias : 'widget.help',
    //title: 'Gramps help',
	tbar: [
		{xtype: 'combo',
/*    		fieldLabel: 'Topic',
    		labelAlign: 'right',
    		labelWidth: 60,
*/
			emptyText: 'Select a topic...',
	    	displayField: 'name',
	    	valueField: 'link',
	    	store: 'Topics',
	    	name: 'topics',
	    	bubbleEvents: ['select', 'blur'] // up to parent panel
	    },
		//{xtype: 'tbfill'},
	    '-',
		{xtype: 'combo' ,
/*			fieldLabel: 'Command',
			labelAlign: 'right',
			labelWidth: 60,
*/
			emptyText: 'Select a command...',
	    	displayField: 'name',
	    	valueField: 'link',
	    	store: 'Commands',
	    	name: 'commands',
	    	bubbleEvents: ['select', 'blur'] // up to parent panel
	    }
    ],
	html: '<iframe id="helpdocs" style="overflow:auto;width:100%;height:100%;"></iframe>',
    initComponent: function() {
        this.callParent(arguments);
    }
});