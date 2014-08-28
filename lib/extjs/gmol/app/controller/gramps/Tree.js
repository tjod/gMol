Ext.define('Gmol.controller.gramps.Tree', {
    extend: 'Ext.app.Controller',
    stores: ['gramps.Tree'],
    //models: ['gramps.Tree'],
    views: ['gramps.Tree'],
    refs:[ {ref:'grampsTree', selector:'grampstree'} ], // reference the instanciated view               
    init: function() {
		this.control({
            //'viewport > tabpanel > panel[title="Tree"]': {
			grampstree: {
                render: this.onPanelRendered,
                show: this.onPanelShown,
                expand: this.onPanelExpand,
                itemdblclick: this.onItemDblClick,
                focus: this.onFocus
            }
        });
        // signal gmol.molAdded goes to this slot/function
        // advance in zoom.gin after newmol causes problems here
        // adv 20 was commented out - not needed anyway
        if (typeof(gmol) !== 'undefined') gmol.molAdded.connect(this.showtree);              
    },
               
    showtree: function(x) {
       // this gets called with scope at top (this == window) level
       // as a qt slot via a qt signal, so this.xxx is not like other callbacks.
       // get the view instance referenced as grampsTree in refs above
       var tree = Gmol.app.getGrampsTreeController().getGrampsTree();
       //console.log("new Mol " + x);
       //console.log(tree);
       tree.refresh(x);
    },
               
    onPanelRendered: function(tree, opt) {
        console.log(tree.title + ' panel was rendered');
    },
    
    onPanelShown: function(tree, opt) {
        console.log(tree.title + ' panel was shown');
        tree.refresh(opt);
        //var newroot = tree.getStore().parseTree();
   		//tree.setRootNode(newroot);
   		//tree.expandAll();
    },
    
    onFocus: function(tree, event, opt) {
        console.log(tree);
    },

    onPanelExpand: function(tree, opt) {
      console.log(tree.title + ' expanded');
    },

    onItemDblClick: function(view, record, item, index, e, opt) {
        //console.log('double clicked ' + index);
        //console.log(' item ' + item + ' was clicked');
        this.showtree(opt);
    }

});
