Ext.define('Gmol.controller.gmol.Tree', {
    extend: 'Ext.app.Controller',
    stores: ['gmol.Tree'],
    models: ['gmol.Tree'],
    views:  ['gmol.Tree'],
    refs:[ {ref:'gmolTree', selector:'gmoltree'} ], // reference the instanciated view
    init: function() {
		this.control({
            //'viewport > tabpanel > panel[title="Tree"]': {
			gmoltree: {
                render: this.onPanelRendered,
                show:   this.onPanelShown,
                //expand: this.onPanelShown,  // when accordion expands, not just shown
                //activate:   this.onPanelActivated,
                itemcontextmenu: this.onItemClick
            }
        });
        // signal gmol.molAdded goes to this slot/function                          
        if (typeof(gmol) !== 'undefined') gmol.molAdded.connect(this.showtree);
    },

    showtree: function(x) {
        // this gets called with scope at top level as a qt slot due to a qt signal
        //var tree = Gmol.app.controllers.get('gmol.Tree').getMolTree();
        // get the view instance referenced as gmolTree
        var tree = Gmol.app.getGmolTreeController().getGmolTree();
        //console.log("new Mol " + x);
        tree.refresh(x);
    },

    onPanelRendered: function(tree, opt) {
        console.log(tree.title + ' panel was rendered');                         
    },
    onPanelActivated: function(tree, opt) {
        console.log(tree.title + ' panel was activated');
    },

	contextMenu: Ext.create('Ext.menu.Menu', {
		title: 'My menu',
	    items: [
            {menuid: 'zoom',   text: 'Zoom'},
	    	{menuid: 'center', text: 'Center'},
	    	{menuid: 'delete', text: 'Delete'}
	    ],
	    listeners: {
	        itemclick: function(item) {
	            switch (item.menuid) {
	                case 'zoom':
	                    break;
	                case 'center':
	                	break;
	                case 'delete':
	                	break;
	            }
	        }
	    }
	}),

    onItemClick: function(view, rec, item, idx, event, opt) {
    	event.stopEvent();  // let me handle this even
    	console.debug(rec);
    	var x = event.browserEvent.clientX;
        var y = event.browserEvent.clientY;
        this.contextMenu.setTitle(rec.get('rowname'));
    	this.contextMenu.showAt([x,y]);
    },

    onPanelShown: function(tree, opt) {
        console.log(tree.title + ' panel was shown');
        tree.refresh(opt);
    },

    showContextMenu: function(row) {
    	Ext.Msg.alert('Clicked', row.grampsname);
    }

});
