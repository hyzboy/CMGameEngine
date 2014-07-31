object SelectFolderForm: TSelectFolderForm
  Left = 334
  Top = 177
  Caption = #35831#36873#25321#19968#20010#30446#24405
  ClientHeight = 421
  ClientWidth = 338
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #26032#23435#20307
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 12
  object Panel1: TPanel
    Left = 0
    Top = 385
    Width = 338
    Height = 36
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object Panel2: TPanel
      Left = 196
      Top = 0
      Width = 142
      Height = 36
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object Button1: TButton
        Left = -1
        Top = 6
        Width = 67
        Height = 23
        Caption = #30830#23450
        TabOrder = 0
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 72
        Top = 6
        Width = 65
        Height = 23
        Caption = #21462#28040
        TabOrder = 1
        OnClick = Button2Click
      end
    end
    object Button3: TButton
      Left = 8
      Top = 6
      Width = 75
      Height = 23
      Caption = #24314#31435#23376#30446#24405
      TabOrder = 1
      OnClick = Button3Click
    end
  end
  object DirectoryListBox: TDirectoryListBox
    Left = 0
    Top = 18
    Width = 338
    Height = 367
    Align = alClient
    ItemHeight = 16
    TabOrder = 1
    ExplicitTop = 152
    ExplicitHeight = 233
  end
  object DriveComboBox1: TDriveComboBox
    Left = 0
    Top = 0
    Width = 338
    Height = 18
    Align = alTop
    DirList = DirectoryListBox
    TabOrder = 2
    ExplicitLeft = 72
    ExplicitTop = 40
    ExplicitWidth = 145
  end
end
