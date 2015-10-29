
Ext.define('MyDesktop.App', {
    extend: 'Ext.ux.desktop.App',

    requires: [
        'Ext.window.MessageBox',
        'Ext.ux.desktop.ShortcutModel',
        'MyDesktop.DwglWindow',
        'MyDesktop.GdrzWindow',
        'MyDesktop.ZxlWindow',
        'MyDesktop.DwzWindow',
        'MyDesktop.BkglWindow',
        'MyDesktop.BkgjWindow',
        'MyDesktop.ZxdwWindow',
        'MyDesktop.Settings'
    ],

    init: function() {
        // custom logic before getXYZ methods get called...

        this.callParent();

        // now ready...
    },

    getModules : function(){
        return [   
            new MyDesktop.DwglWindow(),
            new MyDesktop.GdrzWindow(),
            new MyDesktop.ZxlWindow(),
            new MyDesktop.ZxdwWindow(),
            new MyDesktop.BkgjWindow(),
            new MyDesktop.BkglWindow(),
            new MyDesktop.DwzWindow()
           
        ];
    },

    getDesktopConfig: function () {
        var me = this, ret = me.callParent();

        return Ext.apply(ret, {
            //cls: 'ux-desktop-black',

            contextMenuItems: [
                { text: 'Change Settings', handler: me.onSettings, scope: me }
            ],

            shortcuts: Ext.create('Ext.data.Store', {
                model: 'Ext.ux.desktop.ShortcutModel',
                data: [
                  
                    { name: '�鵵��־', iconCls: 'grid-shortcut', module: 'gdrz-win' },
                    { name: '��λ����', iconCls: 'accordion-shortcut', module: 'dwgl-win' },
                    { name: '������ͳ��', iconCls: 'cpu-shortcut', module: 'zxl-win' },
                    { name: '���ع���', iconCls: 'notepad-shortcut', module: 'bkgl-win' },
                    { name: '���ظ澯��ѯ', iconCls: 'grid-shortcut', module: 'bkgj-win' },
                    { name: '��λ�����', iconCls: 'grid-shortcut', module: 'dwz-win' },
                    { name: '���ߵ�λ', iconCls: 'grid-shortcut', module: 'zxdw-win' },
                    { name: 'ϵͳ�û�', iconCls: 'grid-shortcut', module: 'xtyh-win' },
                    { name: '��ɫ����', iconCls: 'grid-shortcut', module: 'jsgl-win' }
                ]
            }),

            wallpaper: '/newver/resources/windows/wallpapers/Blue-Sencha.jpg',
            wallpaperStretch: false
        });
    },

    // config for the start menu
    getStartConfig : function() {
        var me = this, ret = me.callParent();

        return Ext.apply(ret, {
            title: 'ϵͳ����Ա',
            iconCls: 'user',
            height: 300,
            toolConfig: {
                width: 100,
                items: [
                    {
                        text:'����',
                        iconCls:'settings',
                        handler: me.onSettings,
                        scope: me
                    },
                    '-',
                    {
                        text:'�˳�',
                        iconCls:'logout',
                        handler: me.onLogout,
                        scope: me
                    }
                ]
            }
        });
    },

    getTaskbarConfig: function () {
        var ret = this.callParent();

        return Ext.apply(ret, {
            quickStart: [
               
                { name: '��λ����', iconCls: 'accordion', module: 'dwgl-win' }
            ],
            trayItems: [
                { xtype: 'trayclock', flex: 1 }
            ]
        });
    },

    onLogout: function () {
        Ext.Msg.confirm('�˳�', '��ȷ��Ҫ�˳�ϵͳ��?');
    },

    onSettings: function () {
        var dlg = new MyDesktop.Settings({
            desktop: this.desktop
        });
        dlg.show();
    }
});
