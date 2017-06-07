Ext.require('Ext.container.Viewport');
Ext.require('Ext.tab.Panel');
Ext.require('Ext.layout.container.Accordion');
Ext.require('Ext.data.TreeStore');

Ext.application({
    name: 'Gramps',
    appFolder: 'app',
    controllers: [
    	'Tree', 'System', 'Commands'//, 'Demo'
    ],
    launch: function() {
        Ext.create('Ext.container.Viewport', {
        	layout: 'fit',
            items: {
            	xtype: 'tabpanel',
            	//layout: 'accordion',
            	//title: 'Gramps web 1.0', // let the qt title take it's place
            	items: [
            		Gramps.view.Commands,
            		Gramps.view.Tree,
            		Gramps.view.System,
            		//Gramps.view.Demo
            	]
            }
        });
    }
});