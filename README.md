gohan (Arduinoロボット)
==========  

![gohan and susumu](https://github.com/akinorihomma/gohan/blob/master/images/gohan_and_susumu.jpg)

はじめに
----------
### 概要
平成26年年度
茨城大学 工学部 知能システム工学科 3年次  
科目名: 知能システム工学実験  
F班  

**学科内の** ロボコンで使用する予定のキャタピラで走行する Arduino ロボットのリポジトリです.  
そのロボットの名前が **「ゴハン( gohan )」** です.  

班員以外の人への情報共有の意味合いで公開しております. (多分見てないだろうけど)  
もっとも, 実験終了後は次年度の参考資料になればと思っています.  
実験の相談にも乗りますのでメールやPull Requestなどをくれれば, 相談にものります.

### 関係者各位(後輩とか)
**情報系を目指している人にも Arduino はマイコンを理解する上でとても役にたちます.**  
今年度(H26)以降はどのような実験になるか分かりませんが, 自前で Arduino を買っても損はありません.  

Arduino の開発環境はとても優しいものです.  
今回の学生実験を通して, **実感した**メリットは以下のとおり
+ Java Runtime Environment が動作する環境ならば OS を問わない
+ サンプルスケッチ(プログラム)が豊富
+ コンパイルから転送まで簡単
+ shield(シールド)で簡単に機能を追加
+ シールドを使用せずにモータードライバなどの論理ICも使用可能
+ Android 端末との連携も可能(今回はできなかったけど)
+ [Processing](https://processing.org/) と連携による数値のグラフ化

来年度も Arduino を使うといいなぁ...

機能の概要
----------
1. 直進する
2. 壁があれば左に90度旋回
3. 2回目の壁を検知したときサーボモータを制御してアームを下ろす  
4. アームを下ろすタイミングは H8 の主要プログラム終了時  

4 については最後に記載します.

ゴハンの主なパーツ
---------
+ Arduino UNO
+ ~~Arduino Motor Shield~~
+ Adafruit Motor Shield Kit v2.0
+ ダブルギアボックス(タミヤ製)
+ 距離センサ(HC-SR04)
+ サーボモータ  

また Adafruit Motor Shield Kit v2.0 を使用するにあたり,  
使用したライブラリは以下のものを使用しました.
(https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)[https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library]

進捗状況
----------

### 2015年1月22日
**ロボットコンテスト当日**
+ ライントレースロボットのスピード賞  
+ 知能システム大賞  
+ 参加学生最高評価賞  
の **3冠を達成. 相談にのってもらった各先生やTAの方, 他の班のPLの方に感謝です.**  
なにより **我がチーム** とH8 側のプログラムを作成した **コアプログラマには深い感謝します.**  
競争だから情報はクローズドにするのではなく, 競争相手でも協力するべきだと実験を通して痛感しました.  
![billboard](https://github.com/akinorihomma/gohan/blob/master/images/linetrace_billboard.jpg)

### 2015年1月21日  
主要プログラムの開発終了  
本番まで微調整のみになる  

### 2014年12月22日
直進から旋回までの工程を達成!  
ただし旋回は **時間** で制御するという悲しいことに...  
距離センサをロボット先頭に固定した状況だと, **45度以下の鋭角に対応できない** ため,  
実現が容易な時間制御の旋回を用いた.  
環境, とくに電源周りによって ``` delay() ``` を大きくしたり, 小さくする必要がある.  
~~今更ながらジャイロセンサを使うのが手っ取り早いです~~  

H8 との連携について
----------

### 概要

H8 側ですべてのプログラムが終了したとき, 電圧が HIGH になり Arduino はそれを感知してボタンを押します.

たしかに連携手法は多いですが有線接続に限定すると, 

1. シリアル通信  
2. 電圧の HIGH or LOW  

の2つが考えられます.  
悩んだ末に, H8 側でタスクを完了したかどうかの判断だけで十分なので, 後者の簡単な方を選択しました.  

### 接続とポートの設定
H8 側のポート設定において, P4 はモータ出力のためにポート設定が完了しているので P4 の空きポートを利用します.  
H8 のポート設定については仕様書やヘッダファイルを参照してください.  

H8 ボード上の ch2 の 10 番からコードを伸ばして Arduino の D3 ポートに接続するようにプログラムはなっています.  
Arduino 側のプログラムは gohan.ino を見ていただくとして, H8 側のプログラムを簡単に説明します.  
以下がプログラムの例です.  
``` C
#define Arduino_RUN     P4.DR.BIT.B5 = 0
#define Arduino_STOP    P4.DR.BIT.B5 = 1

main()
{
    Aruino_STOP;

    {主要プログラム}

    Arduino_RUN;
    sleep(1000);

    {more codes}
}
```

H8 の ch2 10番ポートは ```P4.DR.BIT.P5``` に該当します.  
いちいち ```P4.DR.BIT.B5``` うんぬんを記述するのは面倒なので ```#define``` をプログラムの最初に記述, もしくはヘッダファイルで定義します.  
ランダムな出力をされても困るので, 関数では最初に ```P4.DR.BIT.B5 = 0``` で電圧を LOW にしています.  
そして主要プログラムの終了後に ```P4.DR.BIT.B5 = 1``` で電圧を HIGH で出力します. 
```sleep``` はちょっとした気持ちです.  
最後の more codes の部分には ```exit(0)``` などでプログラムを終了させました.  

また Arduino と H8 で電源供給が別ソースの場合はGNDを接続してください.  
電圧はあくまで電位**差**ですので GND が統一されないとデタラメな値を読み取ってしまいます.  
「平地と山で同じ1メートルの高さでも海抜からの高さは違うでしょ」というお話です.  
もし Arudinoと H8 で同一電源を使用しているならば心配は要りません.  


3D モデルについて
----------
この実験では3Dプリンタを多用しました.  
3D モデルを 0 から作るのは骨が折れる作業です.  
GitHub のように **fork** すると手間が省けますし, なによりも 0 から作るよりも今の時代は重要です.  

3D版のGitHub的な存在, [www.thingiverse.com](http://www.thingiverse.com) からモデルを拝借しました.  
どちらもライセンスは CC-BY-SA です.  

**gohan にArduinoをマウントするために使用**  
Arduino Uno Mounter:   http://www.thingiverse.com/thing:625018  
*Arduino Uno Mounter by komix is licensed under the Creative Commons - Attribution - Share Alike license.*  

**gohan で使用した追加のスペーサー**  
5.7 mm x 50mm hex spacer:   http://www.thingiverse.com/thing:547940  
*5.7mm x 50mm hex spacer by theviper0308 is licensed under the Creative Commons - Attribution - Share Alike license.*  


LICENSE
----------
[MIT License](http://ja.wikipedia.org/wiki/MIT_License)  
端的に言えば, 「ご自由にお使いください」  
でも免責は負いません.  

ただし, Adafruit Industry の Adafruit_Motor_Shield_V2_Library は  
Adafruit Industry 社の著作物であり, BSD ライセンスのもとにあります.  

詳細URLは LICENSE に記載してあります.


Copyright (c) 2014 calendaaaaar
