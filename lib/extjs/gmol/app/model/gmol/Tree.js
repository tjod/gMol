Ext.define('Gmol.model.gmol.Tree', {
    extend: 'Ext.data.Model',
    fields: [
             {name: 'grampsname',  type: 'string'},
             {name: 'rowname',     type: 'string'},
             {name: 'itemid',      type: 'int'},
             {name: 'parentid',    type: 'int'},
             {name: 'imol',        type: 'int'},
             {name: 'hydrogens',   type: 'boolean', defaultValue: false},
             {name: 'chain',       type: 'string'},
             {name: 'colorBy',     type: 'int'},
             {name: 'filter',      type: 'int'},
             {name: 'hue',         type: 'int'},
             {name: 'saturation',  type: 'int'},
             {name: 'value',       type: 'int'},
             {name: 'ignore',      type: 'boolean', defaultValue: false},
             {name: 'resnum',      type: 'int'},
             {name: 'style',       type: 'int'} 
     ]
});