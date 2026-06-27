## 日本人の顔のマハラノビス _D_<sup>2</sup>

### 概要
日本人の顔について、19の人類学的計測指(示)数のマハラノビス汎距離 _D_ の二乗を求めるための Excel と LibreOffice Calc ファイルです。  
_D_<sup>2</sup> が大きな顔は平均顔から離れた顔で、_D_<sup>2</sup> の小さな顔は平均顔に近い顔です。  
_D_<sup>2</sup> は、指数間の相関を考慮した距離であるため、「幅広い顔で幅広い鼻」の顔と「幅広い顔で幅が狭い鼻」では、後者の方が大きな値となります。（∵顔の幅と鼻の幅は正の相関があるため）

### 使い方
ここでは pixXY を使った方法を述べます。PixXYの使い方は、[README_JP.md](https://github.com/brhr-iwao/pixxy/blob/master/README_JP.md) をご覧ください。
1. Excel ファイル [jpface_mahalanobis_d2.xlsm](https://github.com/brhr-iwao/pixxy/blob/master/jpface_mahalanobis_d2/jpface_mahalanobis_d2.xlsm) または LibreOffice Calc ファイル  [jpface_mahalanobis_d2.ods](https://github.com/brhr-iwao/pixxy/blob/master/jpface_mahalanobis_d2/jpface_mahalanobis_d2.ods) を、ご自身に環境に応じて取得します。
2. 1のファイルを開き、マクロを有効にします。  
![Excel](https://github.com/brhr-iwao/pixxy/blob/master/images/excel_enable_macro.png)
![LibreOffice](https://github.com/brhr-iwao/pixxy/blob/master/images/libre_enable_macro.png)  
「人類学的標記点」シートをアクティブにし、ドロップダウンリストで男女を選択します。    
![人類学的標記点シート](https://github.com/brhr-iwao/pixxy/blob/master/images/select_landmark_sheet_and_sexuality.png)  
pixXYで顔画像を開き、標記点のXY座標を「人類学的標記点」の所定のセルに入力します。（#N/Aの上に上書きします）  
> [!NOTE]
> 口を開いた顔では「口裂正中点」に、正中線上の上赤唇縁下端（ウワクチビルの下端点）を、「口裂正中点（下赤唇）」に下赤唇縁上端（シタクチビルの上端点）を入力してください。口を閉じた顔では、「口裂正中点」の入力のみで（「口裂正中点（下赤唇）」は#N/Aのままで）OKです。  
![口裂正中点](https://github.com/brhr-iwao/pixxy/blob/master/images/stomion.png)  
3. 「人類学的計測指数」シートをアクティブにします。入力した標記点に応じた計測指数が表示されています。  
![人類学的計測指数シート](https://github.com/brhr-iwao/pixxy/blob/master/images/index.png) 
4. _D_<sup>2</sup>計算に利用したい指数をチェックボックスで選択します。チェックボックスをオフにすると、値が入っていたセルは#N/Aに変わります。  
![チェックボックス](https://github.com/brhr-iwao/pixxy/blob/master/images/checkoff.png) 
5. 「人類学的計測指数」シートの「D2を計算」ボタンを押すと、マハラノビス _D_<sup>2</sup> の値が計算、表示されます。  
![D2を計算](https://github.com/brhr-iwao/pixxy/blob/master/images/calculate_d2.png) 
6. __「数字の見方」__  
![計算値](https://github.com/brhr-iwao/pixxy/blob/master/images/calculated_values.png) 
* _D_<sup>2</sup> の期待値は、分布型によらず、有効次元数に等しいです。_D_<sup>2</sup> が有効次元数より大きければ、平均顔から離れた顔、_D_<sup>2</sup> が有効次元数より小さければ、平均顔に近い顔であると、おおよそいえます。
* もし _D_<sup>2</sup> の分布を多変量正規分布とすると（これは厳密には成り立ちませんが、そこそこ良い近似です）「D2最頻値」の値の顔が、もっともよくある顔となります。（？？平凡な顔？？）
* 日本人集団の中で、どの程度の位置にいるかは、「χ2-p値」で示されます。(ただし、正規近似)　もしこの値が0.159だとすると、上位15.9% (偏差値60)くらいの位置にいることを意味します。ただし、この値は、多変量正規分布を前提とした場合の値で、実際の確率値は、特に変数が多い場合、「χ2-p値」より大きくなります。一方、[マルコフの不等式](https://ja.wikipedia.org/wiki/%E3%83%9E%E3%83%AB%E3%82%B3%E3%83%95%E3%81%AE%E4%B8%8D%E7%AD%89%E5%BC%8F)から、この実際の確率値は、分布型によらず、「p値のマルコフ上限」を上回ることはありません。

### 人類学的計測点
[文献1](#文献1)及び[文献2](#文献2)に準拠しています。    
![人類学的計測点](https://github.com/brhr-iwao/pixxy/blob/master/images/landmarks.png) 
| 和名 | ラテン名(略号) | 説明 |
|:----:|:----:|:----|
|鼻根点<br>びこんてん| nasion(n)<br>ナジオン | 頭蓋骨で前頭鼻骨縫合と[正中矢状面](#注1)の交わる点。<br> 顔写真では「眉の最内側点を結ぶ線と正中線の交点」と「内眼角点同士を結ぶ線と正中線の交点」の2点の中間点。<br>[文献3](#文献3)によれば、これらの点は高い相関(R<sup>2</sup>:男0.964、女0.944)を示す。|
||sellion(se)<br>セリオン|鼻根部で、[正中矢状面](#注1)上で最も落ち窪んでいる点。<br> [文献4](#文献4)では鼻根点の代替点として利用。|
|内眼角点<br>ないがんかくてん|entochanthion(en)<br>エントカンチオン|眼瞼裂の内側端点。|
|外眼角点<br>がいがんかくてん|ectochanthion(ex)<br>エクトカンチオン|眼瞼裂の外側端点。|
|頬骨弓点<br>きょうこつきゅうてん|zygion(ex)<br>ジギオン|頬骨弓の最も外側方にある点。<br>[文献4](#文献4)では、[耳眼平面](#注2)と正貌の輪郭線の交点。|
|鼻翼点<br>びよくてん|alare(al)<br>アラーレ|鼻翼(コバナ)の最も外側方に張り出している点。|
|鼻下点<br>びかてん|subnasale(sn)<br>ズブナザーレ|鼻中隔前縁の下端が上唇部の皮膚面(ハナノシタ)に移行する点。|
|口点<br>こうてん|stomion(sto)<br>ストミオン|口を自然に閉じた状態で口裂線の[正中矢状面](#注1)の交点。|
|上唇点<br>じょうしんてん|labrale superius(ls)<br>ラブラーレ・スーペリウス|上赤唇縁（ウワクチビル）の最高点を通る水平線と[正中矢状面](#注1)の交わる点。|
|下唇点<br>かしんてん|labrale inferius(li)<br>ラブラーレ・インフェリウス|下赤唇縁（シタクチビル）の下縁と[正中矢状面](#注1)の交わる点。|
|口角点<br>こうかくてん|cheilion(ch)<br>ケイリオン|口角で上唇（≒ウワクチビル）と下唇（≒シタクチビル）が移行する点。|
|下顎角点<br>かがくかくてん|gonion(go)<br>ゴニオン|下顎骨の下顎角部の最も下方・後方・外側方にある点。<br>[文献4](#文献4)では口裂線の延長線と正貌の輪郭線の交点。|
|オトガイ点|gnathion(gn)<br>グナチオン|[耳眼平面](#注2)を水平にした状態で、[正中矢状面](#注1)で下顎骨下縁の最も下方の点。<br>顔写真では[正中矢状面](#注1)と正貌の輪郭線下縁の交点。|

### 参考文献
<a id="文献1" href="https://dl.ndl.go.jp/pid/12685250">
1. 藤田恒太郎（寺田春水改訂）、生体観察、南山堂、1976年、https://dl.ndl.go.jp/pid/12685250  (国立国会図書館デジタルコレクション送信サービスで閲覧できます)
</a>
<br><br>
<a id="文献2">
2. 人類学講座編集委員会、人類学講座別巻1、人体計測法(I 生体計測法、II 人骨計測法)、雄山閣出版、1991年
</a>
<br><br>
<a id="文献3">
3. 宮坂祥夫、小川好則、谷口慶、今泉和彦、顔写真を用いた人類学的計測検査のためのNasion代替点の設定方法に関する提案、科学警察研究所報告、第63巻第2号、63-73頁、2014年
</a>
<br><br>
<a id="文献4" href="http://dx.doi.org/10.1016/j.forsciint.2015.07.046">
4. Y. Ogawa, B. Wada, K. Taniguchi, S. Miyasaka, K. Imaizumi, Photo anthropometric variations in Japanese facial features: Establishment of large-sample standard reference data for personalidentification using a three-dimensional capture system, Forensic Science International, 257, 511.e1-511.e9 (2015), http://dx.doi.org/10.1016/j.forsciint.2015.07.046
</a>

### 注
<a id="注1" href="#文献2">
1. 正中矢状面とは、身体を左右対称に折半する平面。
</a>
<br>
<a id="注2" href="#文献1">
2. 耳眼平面とは、左右の耳珠点（耳珠上縁の付け根の点）と左の眼窩点（眼窩下縁の最下点）を通る平面。フランクフルト平面、ドイツ平面ともいう。
</a>
