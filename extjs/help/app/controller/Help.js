Ext.define('Help.controller.Help', {
    extend: 'Ext.app.Controller',
    title: 'Help',
    views: ['Navigation'],
    stores: ['Commands', 'Topics'],
    docRoot: '../../gramps/html/',
    init: function() {
		this.control({
			help: {
                render: this.onPanelRendered,
                afterrender: this.onPanelShown,
                select: this.onComboBoxClicked,
                blur: this.onBlurred
            }
        });
    },

    onPanelRendered: function(panel, opt) {
        Ext.log(panel.title + ' panel was rendered');
    },

    onComboBoxClicked: function(cb, val, opt) {
    	Ext.log(cb.name + ":" + cb.value);
		this.displayHelp(cb.value);
    },
    
    onBlurred: function(cb,opt) {
    	cb.reset();
    },
    
    onPanelShown: function(panel, opt) {
        Ext.log(panel.title + ' panel was shown');
        this.displayHelp('introduction.html');
    },
    
    displayHelp: function(htmlFile) {
    	var link = this.docRoot + htmlFile;
        document.getElementById('helpdocs').src = link;
    }
});
