Ext.define('Gramps.controller.System', {
    extend: 'Ext.app.Controller',
    views: [
            'System'
    ],
    init: function() {
		this.control({
            //'viewport > tabpanel > panel[title="System"]': {
			showsystem: {
                render: this.onPanelRendered,
                show: this.onPanelShown,
                expand: this.onPanelShown, // when accordion expands, not just shown
                click: this.onPanelClicked
            },
            'viewport > tabpanel': {
            	render: this.onPanelRendered
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
        panel.update("<PRE>"+gramps.cmd('show/system'));
    }
});