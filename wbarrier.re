= ライトバリアのコスト
いよいよ本書も最後の章となりました。
最後の章ではライトバリアのコストについて取り上げます。

== 実行時のGC切り替え
本書でも説明してきたとおり、HotspotVMは複数のGCアルゴリズムを選択することができます。
しかも、起動オプションとしてGCアルゴリズムを指定しますので、Javaプログラムを実行するときにGCを切り替えなくてはいけません。

=== 性能劣化の懸念
実行時にGCを切り替える以外にも、たとえばコンパイル時にGCを切り替えるという方法があります。
G1GC用にコンパイルしたOpenJDK、CMS用にコンパイルしたOpenJDK、という風に異なるGCアルゴリズムごとにビルドしたバイナリを配布すればよいでしょう。

ですが、この方法だと開発者はGCアルゴリズムを追加するたびにバイナリを増やさないといけません。
また管理するバイナリが増えてしまい、いらない手間がかかってしまうことは容易に想像が付きます。
また、ユーザの利便性という面からあまり現実的ではありません。
やはり、実行時にGCを切り替えつつ、お手軽にいろいろ試したいわけですから。

実行時にGCを切り替える方法はいいことずくめに見えますが、コンパイル時にGCを切り替える方法と比べて性能劣化をまねくという欠点があります。
具体的なサンプルコードとしてC言語で書いたGCを実行する@<code>{gc_start()}という関数を以下に示します。

//list[dynamic_gc_options_with_c][例: 実行時GC切り替えのGC起動関数]{
void
gc_start(gc_state state) {
  switch (state) {
    case gc_state_g1gc;
      g1gc_gc_start();
      break;
    case gc_state_cms;
      cms_gc_start();
      break;
    case gc_state_serial;
      serial_gc_start();
      break;
  }
};
//}

//list[static_gc_options_with_c][例: コンパイル時GC切り替えのGC起動関数]{
void
gc_start(void) {
#ifdef GC_STATE_G1GC
  g1gc_gc_start();
#elif GC_STATE_CMS
  cms_gc_start();
#elif GC_STATE_SERIAL
  serial_gc_start();
#endif
};
//}

@<list>{dynamic_gc_options_with_c}では@<code>{gc_start()}を実行するときに条件分岐の処理が入ってしまいます。
一方、@<list>{static_gc_options_with_c}はコンパイル時に@<code>{gc_start()}内で呼び出す関数が決定するので、実行時に条件分岐の処理が不要です。

=== ライトバリアのコスト増加
実行時にGCを切り替える際、もっとも性能劣化が懸念される場所はライトバリアです。
ライトバリアは頻繁に実行され、ボトルネックになりやすい処理です。
GCを実行時に切り替える場合は、GCにあったライトバリアを実行時に切り替えなければなりません。
@<list>{dynamic_gc_options_with_c}で説明した条件分岐のような切り替えが必要になってくるわけです。
そのため、ライトバリアに余計にコストがかかってしまい、ミューテータの速度に影響がでてしまいます。

== インタプリタのライトバリア

=== オブジェクトフィールドへの格納

=== ライトバリアの実装

=== 仮想関数のオーバーヘッド

== JITコンパイラのライトバリア

=== C1コンパイラ

=== ライトバリアのマシン語生成

== おわりに
