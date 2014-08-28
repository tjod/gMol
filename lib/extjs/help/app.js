Ext.require('Ext.container.Viewport');
Ext.require('Ext.form.ComboBox');

Ext.application({
    name: 'Help',
    appFolder: 'app',
    controllers: ['Help'],
    launch: function() {
        Ext.create('Ext.container.Viewport', {
        	layout: 'fit',
			defaults: {bodyPadding: 10},
            items: [Help.view.Navigation]
        });
    }
});