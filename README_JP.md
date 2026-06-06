## pixXY

### 概要
PixXYは画像のピクセル座標をExcelスプレッドシートにコピー＆ペーストするためのツールです。PixXYはピクシーと読みます。

### インストール
Windowsバイナリはインストールの必要がありません。「pixxy.exe」アイコンをダブルクリックして起動します。「pixxy.exe」は"[Releases](https://github.com/brhr-iwao/pixxy/releases)" のzipファイルにパックされています。

PixXYの起動時に「アプリケーションの構成が正しくないため、アプリケーションを開始できませんでした。再インストールすることで問題が解決する可能性があります」のエラーが発生する場合は、 [VC2005 sp1用のvcredist_x86.exe(Microsoft Visual C++ 2005 SP1 再頒布可能パッケージ (x86) )](https://github.com/brhr-iwao/pixxy/releases/download/1.0.190707/vcredist_x86.exe)をインストールしてください。

### 使い方
1. __「画像を開く」__ PixXYを起動し、「File(F) --> Open Image(O)...」から画像ファイルを開きます。
2. __「ピクセル位置のコピー」__　マウスカーソル矢印の先端を、画像上のXY座標を知りたい位置に置きます。
ウィンドウ左下のステータスバーにXY座標が表示されます。
右クリックでポップアップメニューから「Copy the coordinate Ctrl + C」を実行すると、「整数値　タブ　整数値」の座標値がクリップボードにコピーされます。
3. __「ピクセル位置をペースト」__ 　座標値はお好みのスプレッドシートやエディターにペーストして利用することができます。

### 人類学的写真計測
#### Excelスプレッドシートファイル"[face_measurement_jp.xls](https://github.com/brhr-iwao/pixxy/blob/master/anthropometry%20examples/face_measurement_jp.xls)"について.
1. ファイルは、「計測点」「計測長」「計測示数」の3つのシートから構成されています。Microsoft ExcelやLibreOffice Calcなどで開くことができます。
2. 顔画像をPixXYで開きます。  
![画像を開く](https://github.com/brhr-iwao/pixxy/blob/master/images/open_image.png)
3. 顔画像の人類学的標記点の上のXY座標をコピーし、「計測点」シートの適切なセルにペーストします（#N/Aの上に上書きします）。    
![右クリックメニュー](https://github.com/brhr-iwao/pixxy/blob/master/images/point_and_popuup_menu.png)
![座標を貼り付け](https://github.com/brhr-iwao/pixxy/blob/master/images/paste_the_coordinate_jp.png)
4. 必要な人類学的標記点の分だけ、上の3を繰り返します。示数は自動的に「計測示数」シートに表示されます。
5. 上の4までで、いくつかの人類学的な分類(広顔、中顔、狭顔、広鼻、中鼻、狭鼻)が行われています。
6. 「示数」シートの「性別」ドロップダウンリストから「男」を選んでいる場合、「日本人」「ドイツ人」「イタリア人」「リトアニア人」のうちで人類学的示数上、最も近い民族が表示されています。

#### 例
1. バラク・オバマ氏  
![バラク・オバマ氏](https://github.com/brhr-iwao/pixxy/blob/master/images/barak-obama.png)


オバマ氏は人類学的顔指数の観点からはとても狭い顔といえます。ただし、写真の顔は笑っており、わずかに口を開いていることを考慮しなければなりません。口を開いていることで、高い顔面高となります。  
オバマ氏は相貌学的上顔指数（口を開くことの影響はありません）では中顔です。また笑顔のため広い鼻幅となっています。

2. ドナルド・トランプ氏  
![ドナルド・トランプ氏](https://github.com/brhr-iwao/pixxy/blob/master/images/donald-trump.png)

   人類学的計測（全顔指数とen-en:al-al:ch-chの関係）ではトランプ氏は日本人に類似しています

これらの画像（オバマ氏は笑っているし、トランプ氏は斜め側貌で軽く口を開いているし）はあまり適切ではなかったですね...

### 免責事項
PixXYは、[stbライブラリ](https://github.com/nothings/stb) と同じくパブリックドメインです。PixXYは一切の保証なしに提供されます。PixXYの使用によって得られる結果について、いかなる保証もありません。

### 謝辞
きわめてプログラマーフレンドリーな画像読み書きライブラリである [stbライブラリ](https://github.com/nothings/stb)を開発・公開いただいた [Sean Barrete](https://nothings.org/) 様に深謝します。また[画像プレビュー付き WIN32 コモンダイアログ](https://www.petitmonte.com/c_cplus/capi_sample52.html)をご提示いただいた[岡本武史](https://www.petitmonte.com/) 様にも感謝いたします。

### 連絡先
[Aoyama Iwao](https://github.com/brhr-iwao)
