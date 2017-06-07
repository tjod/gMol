Ext.define('Gramps.controller.Commands', {
    extend: 'Ext.app.Controller',
    views: [
            'Commands'
    ],
    init: function() {
		this.control({
            //'viewport > tabpanel > panel[title="Commands"]': {
			commands: {
                render: this.onPanelRendered
            },
            'commands > textfield': {
            	specialkey: this.onSpecialKey
            }
        });
    },

    onPanelRendered: function(panel, opt) {
        Ext.log(panel.title + ' panel was rendered');
    },
    
    onSpecialKey: function(field, e) {
        if (e.getKey() == e.ENTER) {
        	var cmd = field.getValue();
        	if (cmd.length > 0) {
        		var result = gramps.cmd(cmd);
        		var cmdout = field.next(); // the sibling textarea
        		cmdout.setValue(">"+cmd+"\n"+(result.length>0?result+"\n":"")+cmdout.getValue());
        		if (result[0] != "^") field.setValue("");
        	}
        }
    }

});