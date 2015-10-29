/*
This file is part of Ext JS 4.2

Copyright (c) 2011-2013 Sencha Inc

Contact:  http://www.sencha.com/contact

GNU General Public License Usage
This file may be used under the terms of the GNU General Public License version 3.0 as
published by the Free Software Foundation and appearing in the file LICENSE included in the
packaging of this file.

Please review the following information to ensure the GNU General Public License version 3.0
requirements will be met: http://www.gnu.org/copyleft/gpl.html.

If you are unsure which license is appropriate for your use, please contact the sales department
at http://www.sencha.com/contact.

Build date: 2013-05-16 14:36:50 (f9be68accb407158ba2b1be2c226a6ce1f649314)
*/
/**
 * Simplified Chinese translation
 * By DavidHu
 * 09 April 2007
 *
 * update by andy_ghg
 * 2009-10-22 15:00:57
 */
Ext.onReady(function() {
    var parseCodes;

    if (Ext.Date) {
        Ext.Date.monthNames = ["һ��", "����", "����", "����", "����", "����", "����", "����", "����", "ʮ��", "ʮһ��", "ʮ����"];

        Ext.Date.dayNames = ["������", "����һ", "���ڶ�", "������", "������", "������", "������"];

        Ext.Date.formatCodes.a = "(this.getHours() < 12 ? '����' : '����')";
        Ext.Date.formatCodes.A = "(this.getHours() < 12 ? '����' : '����')";

        parseCodes = {
            g: 1,
            c: "if (/(����)/i.test(results[{0}])) {\n"
                + "if (!h || h == 12) { h = 0; }\n"
                + "} else { if (!h || h < 12) { h = (h || 0) + 12; }}",
            s: "(����|����)",
            calcAtEnd: true
        };

        Ext.Date.parseCodes.a = Ext.Date.parseCodes.A = parseCodes;
    }

    if (Ext.util && Ext.util.Format) {
        Ext.apply(Ext.util.Format, {
            thousandSeparator: ',',
            decimalSeparator: '.',
            currencySign: '\u00a5',
            // Chinese Yuan
            dateFormat: 'y��m��d��'
        });
    }
});

Ext.define("Ext.locale.zh_CN.view.View", {
    override: "Ext.view.View",
    emptyText: ""
});

Ext.define("Ext.locale.zh_CN.grid.plugin.DragDrop", {
    override: "Ext.grid.plugin.DragDrop",
    dragText: "ѡ���� {0} ��"
});

Ext.define("Ext.locale.zh_CN.tab.Tab", {
    override: "Ext.tab.Tab",
    closeText: "�رմ˱�ǩ"
});

Ext.define("Ext.locale.zh_CN.form.field.Base", {
    override: "Ext.form.field.Base",
    invalidText: "����ֵ�Ƿ�"
});

// changing the msg text below will affect the LoadMask
Ext.define("Ext.locale.zh_CN.view.AbstractView", {
    override: "Ext.view.AbstractView",
    loadingText: "�xȡ��..."
});

Ext.define("Ext.locale.zh_CN.picker.Date", {
    override: "Ext.picker.Date",
    todayText: "����",
    minText: "���ڱ��������С��������",
    //update
    maxText: "���ڱ���С�������������",
    //update
    disabledDaysText: "",
    disabledDatesText: "",
    nextText: '�¸��� (Ctrl+Right)',
    prevText: '�ϸ��� (Ctrl+Left)',
    monthYearText: 'ѡ��һ���� (Control+Up/Down ���ı����)',
    //update
    todayTip: "{0} (�ո��ѡ��)",
    format: "y��m��d��",
    ariaTitle: '{0}',
    ariaTitleDateFormat: 'Y\u5e74m\u6708d\u65e5',
    longDayFormat: 'Y\u5e74m\u6708d\u65e5',
    monthYearFormat: 'Y\u5e74m\u6708',
    getDayInitial: function (value) {
        // Grab the last character
        return value.substr(value.length - 1);
    }
});

Ext.define("Ext.locale.zh_CN.picker.Month", {
    override: "Ext.picker.Month",
    okText: "ȷ��",
    cancelText: "ȡ��"
});

Ext.define("Ext.locale.zh_CN.toolbar.Paging", {
    override: "Ext.PagingToolbar",
    beforePageText: "��",
    //update
    afterPageText: "ҳ,�� {0} ҳ",
    //update
    firstText: "��һҳ",
    prevText: "��һҳ",
    //update
    nextText: "��һҳ",
    lastText: "���ҳ",
    refreshText: "ˢ��",
    displayMsg: "��ʾ {0} - {1}������ {2} ��",
    //update
    emptyMsg: 'û������'
});

Ext.define("Ext.locale.zh_CN.form.field.Text", {
    override: "Ext.form.field.Text",
    minLengthText: "�����������С������ {0} ���ַ�",
    maxLengthText: "�����������󳤶��� {0} ���ַ�",
    blankText: "��������Ϊ������",
    regexText: "",
    emptyText: null
});

Ext.define("Ext.locale.zh_CN.form.field.Number", {
    override: "Ext.form.field.Number",
    minText: "�����������Сֵ�� {0}",
    maxText: "������������ֵ�� {0}",
    nanText: "{0} ������Ч��ֵ"
});

Ext.define("Ext.locale.zh_CN.form.field.Date", {
    override: "Ext.form.field.Date",
    disabledDaysText: "����",
    disabledDatesText: "����",
    minText: "������������ڱ����� {0} ֮��",
    maxText: "������������ڱ����� {0} ֮ǰ",
    invalidText: "{0} ����Ч������ - ������ϸ�ʽ�� {1}",
    format: "y��m��d��"
});

Ext.define("Ext.locale.zh_CN.form.field.ComboBox", {
    override: "Ext.form.field.ComboBox",
    valueNotFoundText: undefined
}, function() {
    Ext.apply(Ext.form.field.ComboBox.prototype.defaultListConfig, {
        loadingText: "������..."
    });
});

Ext.define("Ext.locale.zh_CN.form.field.VTypes", {
    override: "Ext.form.field.VTypes",
    emailText: '������������ǵ����ʼ���ַ����ʽ�磺 "user@example.com"',
    urlText: '�������������URL��ַ����ʽ�磺 "http:/' + '/www.example.com"',
    alphaText: '��������ֻ�ܰ��������ĸ��_',
    alphanumText: '��������ֻ�ܰ��������ĸ,���ֺ�_'
});

//add HTMLEditor's tips by andy_ghg
Ext.define("Ext.locale.zh_CN.form.field.HtmlEditor", {
    override: "Ext.form.field.HtmlEditor",
    createLinkText: '��ӳ�������:'
}, function() {
    Ext.apply(Ext.form.field.HtmlEditor.prototype, {
        buttonTips: {
            bold: {
                title: '���� (Ctrl+B)',
                text: '��ѡ�е���������Ϊ����',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            italic: {
                title: 'б�� (Ctrl+I)',
                text: '��ѡ�е���������Ϊб��',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            underline: {
                title: '�»��� (Ctrl+U)',
                text: '����ѡ���ּ��»���',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            increasefontsize: {
                title: '��������',
                text: '�����ֺ�',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            decreasefontsize: {
                title: '��С����',
                text: '��С�ֺ�',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            backcolor: {
                title: '�Բ�ͬ��ɫͻ����ʾ�ı�',
                text: 'ʹ���ֿ���ȥ������ӫ������˱��һ��',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            forecolor: {
                title: '������ɫ',
                text: '����������ɫ',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            justifyleft: {
                title: '�����',
                text: '�����������',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            justifycenter: {
                title: '����',
                text: '�����־��ж���',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            justifyright: {
                title: '�Ҷ���',
                text: '�������Ҷ���',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            insertunorderedlist: {
                title: '��Ŀ����',
                text: '��ʼ������Ŀ�����б�',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            insertorderedlist: {
                title: '���',
                text: '��ʼ��������б�',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            createlink: {
                title: 'ת�ɳ�������',
                text: '����ѡ�ı�ת���ɳ�������',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            },
            sourceedit: {
                title: '������ͼ',
                text: '�Դ������ʽչ���ı�',
                cls: Ext.baseCSSPrefix + 'html-editor-tip'
            }
        }
    });
});

Ext.define("Ext.locale.zh_CN.grid.header.Container", {
    override: "Ext.grid.header.Container",
    sortAscText: "����",
    //update
    sortDescText: "����",
    //update
    lockText: "������",
    //update
    unlockText: "�������",
    //update
    columnsText: "��"
});

Ext.define("Ext.locale.zh_CN.grid.PropertyColumnModel", {
    override: "Ext.grid.PropertyColumnModel",
    nameText: "����",
    valueText: "ֵ",
    dateFormat: "y��m��d��"
});

Ext.define("Ext.locale.zh_CN.window.MessageBox", {
    override: "Ext.window.MessageBox",
    buttonText: {
        ok: "ȷ��",
        cancel: "ȡ��",
        yes: "��",
        no: "��"
    }    
});

// This is needed until we can refactor all of the locales into individual files
Ext.define("Ext.locale.zh_CN.Component", {	
    override: "Ext.Component"
});

