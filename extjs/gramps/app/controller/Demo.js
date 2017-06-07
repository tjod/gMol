Ext.define('Gramps.controller.Demo', {
    extend: 'Ext.app.Controller',
    views: [
            'Demo'
    ],
    init: function() {
		this.control({
			demo: {
                render: this.onPanelRendered,
                show: this.onPanelShown,
                click: this.onPanelClicked
            }
        });
    },

    onPanelRendered: function(panel, opt) {
        Ext.log(panel.title + ' panel was rendered');
    },

    onPanelClicked: function(panel, opt) {
        Ext.log(panel.title + ' panel was clicked');
    },
    
    onPanelShown: function(panel, opt) {
        Ext.log(panel.title + ' panel was shown');
    }
});