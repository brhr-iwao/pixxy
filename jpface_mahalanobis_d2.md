## 日本人の顔のマハラノビス _D_<sup>2</sup>

### 概要
日本人の顔について、19の人類学的計測指(示)数のマハラノビス汎距離 _D_ の二乗を求めるための Excel と LibreOffice Calc ファイルです。  
_D_<sup>2</sup> が大きな顔は平均顔から離れた顔で、_D_<sup>2</sup> の小さな顔は平均顔に近い顔です。  
_D_<sup>2</sup> は、指数間の相関を考慮した距離であるため、「幅広い顔で幅広い鼻」の顔と「幅広い顔で幅が狭い鼻」では、後者の方が大きな値となります。（∵顔の幅と鼻の幅は正の相関があるため）

### 使い方
ここでは pixXY を使った方法を述べます。PixXYの使い方は、[README_JP.md](https://github.com/brhr-iwao/pixxy/blob/master/README_JP.md) をご覧ください。
1. Excel ファイル [jpface_mahalanobis_d2.xlsm](https://github.com/brhr-iwao/pixxy/blob/master/jpface_malahanobis_d2/jpface_mahalanobis_d2.xlsm) または LibreOffice Calc ファイル  [jpface_mahalanobis_d2.ods](https://github.com/brhr-iwao/pixxy/blob/master/jpface_malahanobis_d2/jpface_mahalanobis_d2.ods) を、ご自身に環境に応じて取得します。
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