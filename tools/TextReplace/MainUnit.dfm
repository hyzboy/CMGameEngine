object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Text Replace'
  ClientHeight = 502
  ClientWidth = 800
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 800
    Height = 189
    Align = alTop
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 100
      Width = 112
      Height = 13
      Alignment = taRightJustify
      BiDiMode = bdLeftToRight
      Caption = #25552#21462#21518#25991#26412#23384#25918#30446#24405':'
      ParentBiDiMode = False
    end
    object Label2: TLabel
      Left = 24
      Top = 131
      Width = 100
      Height = 13
      Alignment = taRightJustify
      BiDiMode = bdLeftToRight
      Caption = #32763#35793#25991#26412#23384#25918#30446#24405':'
      ParentBiDiMode = False
    end
    object Label3: TLabel
      Left = 12
      Top = 162
      Width = 112
      Height = 13
      Caption = #23548#20837#21518#33050#26412#23384#25918#30446#24405':'
    end
    object Label4: TLabel
      Left = 646
      Top = 12
      Width = 64
      Height = 13
      Caption = #38750#33521#25991#23383#25968':'
    end
    object CharCountLabel: TLabel
      Left = 716
      Top = 12
      Width = 6
      Height = 13
      Caption = '0'
    end
    object Label5: TLabel
      Left = 682
      Top = 33
      Width = 28
      Height = 13
      Caption = #34892#25968':'
    end
    object LineCountLabel: TLabel
      Left = 716
      Top = 33
      Width = 6
      Height = 13
      Caption = '0'
    end
    object OpenOrignScriptButton: TButton
      Left = 8
      Top = 6
      Width = 121
      Height = 25
      Caption = #28155#21152#21407#22987#33050#26412#25991#20214
      TabOrder = 0
      OnClick = OpenOrignScriptButtonClick
    end
    object ParseTextButton: TButton
      Left = 135
      Top = 6
      Width = 90
      Height = 25
      Caption = #25552#21462#25991#26412
      TabOrder = 1
      OnClick = ParseTextButtonClick
    end
    object ReplaceTextButton: TButton
      Left = 135
      Top = 37
      Width = 90
      Height = 25
      Caption = #23548#20837#32763#35793#25991#26412
      TabOrder = 2
      OnClick = ReplaceTextButtonClick
    end
    object OrignTextPathEdit: TEdit
      Left = 130
      Top = 97
      Width = 580
      Height = 21
      TabOrder = 3
    end
    object OrignTextPathBrowserButton: TButton
      Left = 716
      Top = 95
      Width = 75
      Height = 25
      Caption = #27983#35272
      TabOrder = 4
      OnClick = OrignTextPathBrowserButtonClick
    end
    object NewTextPathEdit: TEdit
      Left = 130
      Top = 128
      Width = 580
      Height = 21
      TabOrder = 5
    end
    object NewTextPathBrowserButton: TButton
      Left = 716
      Top = 126
      Width = 75
      Height = 25
      Caption = #27983#35272
      TabOrder = 6
      OnClick = NewTextPathBrowserButtonClick
    end
    object NewScriptPathEdit: TEdit
      Left = 130
      Top = 159
      Width = 580
      Height = 21
      TabOrder = 7
    end
    object NewScriptPathBrowserButton: TButton
      Left = 716
      Top = 157
      Width = 75
      Height = 25
      Caption = #27983#35272
      TabOrder = 8
      OnClick = NewScriptPathBrowserButtonClick
    end
    object OperCheckBox: TCheckBox
      Left = 240
      Top = 10
      Width = 149
      Height = 17
      Caption = #25552#21462#30340#25991#26412#21253#21547#24341#21495
      TabOrder = 9
    end
    object UseOneCheckBox: TCheckBox
      Left = 424
      Top = 10
      Width = 117
      Height = 17
      Caption = #20351#29992#21333#19968#25991#26412#25991#20214
      TabOrder = 10
      OnClick = UseOneCheckBoxClick
    end
    object ClipAscCheckBox: TCheckBox
      Left = 240
      Top = 33
      Width = 169
      Height = 17
      Caption = #23558#23383#31526#20018#21069#21518#30340#33521#25991#21435#25481
      Checked = True
      State = cbChecked
      TabOrder = 11
    end
    object ClearMultiItemCheckBox: TCheckBox
      Left = 424
      Top = 33
      Width = 97
      Height = 17
      Caption = #19981#35760#24405#37325#22797#39033
      Checked = True
      State = cbChecked
      TabOrder = 12
    end
    object Button1: TButton
      Left = 8
      Top = 68
      Width = 121
      Height = 25
      Caption = #28165#31354#25991#20214#21015#34920
      TabOrder = 13
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 135
      Top = 68
      Width = 90
      Height = 25
      Caption = #28165#38500#21069#21518#33521#25991
      TabOrder = 14
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 8
      Top = 37
      Width = 121
      Height = 25
      Caption = #26597#25214#33050#26412#24182#28155#21152
      TabOrder = 15
      OnClick = Button3Click
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 189
    Width = 800
    Height = 313
    Align = alClient
    BorderWidth = 2
    TabOrder = 1
    object ScriptListBox: TListBox
      Left = 3
      Top = 3
      Width = 794
      Height = 307
      Align = alClient
      ItemHeight = 13
      TabOrder = 0
    end
  end
  object OpenTextFileDialog: TOpenTextFileDialog
    Filter = 
      'script (*.cpp;*.src;*.inc;*.stcm2;*.mmm;*.scs)|*.cpp;*.src;*.inc' +
      ';*.stcm2;*.mmm;*.scs|text (*.txt)|*.txt'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Encodings.Strings = (
      'Unicode')
    Left = 120
    Top = 208
  end
end
