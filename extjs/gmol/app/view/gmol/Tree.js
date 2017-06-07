Ext.define('Gmol.view.gmol.Tree' ,{
    extend: 'Ext.tree.Panel',
    store: 'gmol.Tree',
    alias : 'widget.gmoltree',
    title: 'tree',
    autoScroll: true,
    rootVisible: true,
    displayField: 'rowname',
    columns: [
              {text:' Name',       dataIndex: 'rowname',    width: 150, xtype: 'treecolumn'},
              {text: 'Chain',      dataIndex: 'chain',      width: 50, hidden:true},
              {text: 'Residue',    dataIndex: 'resnum',     width: 50, hidden:true},
              {text: 'Gramps',     dataIndex: 'grampsname'           },
              {text: 'Id',         dataIndex: 'itemid',     width: 50, hidden:true},
              {text: 'Mol#',       dataIndex: 'imol',       width: 50, hidden:true},
              {text: 'H',          dataIndex: 'hydrogens',  width: 50, hidden:true},
              {text: 'Color By',   dataIndex: 'colorBy',    width: 50, hidden:true},
              {text: 'Filter',     dataIndex: 'filter',     width: 50, hidden:true},
              {text: 'Hue',        dataIndex: 'hue',        width: 50, hidden:true},
              {text: 'Saturation', dataIndex: 'saturation', width: 50, hidden:true},
              {text: 'Value',      dataIndex: 'value',      width: 50, hidden:true},
              {text: 'Hide',       dataIndex: 'ignore',     width: 50, hidden:true},
              {text: 'Style',      dataIndex: 'style',      width: 50, hidden:true} 
      ],
    
    initComponent: function() {
        this.callParent(arguments);
    },

    refresh: function(opt) {
        var newroot = this.getStore().parseTree();
        this.setRootNode(newroot);
        Ext.each( this.getChecked(), function(node) {node.expand();} );
    }
});
