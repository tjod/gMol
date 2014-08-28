Ext.require('Ext.menu.Menu')
Ext.require('Ext.container.Viewport');
Ext.require('Ext.tab.Panel');
Ext.require('Ext.data.TreeStore');
Ext.require('Ext.data.Model');
Ext.require('Ext.layout.container.Border');
Ext.require('Ext.layout.container.Accordion');

/*gmol = {
	showTree: function() {return true},
	atest:    function() {return true}
}*/

Ext.application({
    name: 'Gmol',
    appFolder: 'app',
    controllers:
    ['gmol.Tree',
    'gmol.System',
    'gramps.Tree',
    'gramps.System',
    'gramps.Commands'
    //'Commands',
    //'Demo'
    ],
    launch: function() {
        Ext.create('Ext.container.Viewport', {
        	layout: 'fit',
			defaults: {bodyPadding: 0},
			items: {xtype: 'tabpanel',
			    //resizeable: true,
			    minTabWidth: 200,
               tabBar: {resizeable: true, layout: { pack: 'center'} }, // center-align tabs
            	items:[
	            	{xtype: 'tabpanel',
	            	    //iconCls: 'gmol',
	            		title: 'gmol',
	        			items:
	        			[
	        				Gmol.view.gmol.Tree,
	        				Gmol.view.gmol.System,
	            			//Gmol.view.Commands,
	            		]
	            	},
	        		{xtype: 'tabpanel',
	        			title: 'gramps',
	        			tabBar: {layout : { pack : 'end'} }, // right-align tabs
	        			items:
	        			[
	        				Gmol.view.gramps.Commands,
	        				Gmol.view.gramps.Tree,
	        				Gmol.view.gramps.System
	            			//Gmol.view.Commands,
	            		]
	        		}
	        	]
            }
        });
    }
});
