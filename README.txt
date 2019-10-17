以下全て0-indexed

変数

int  : height 縦の大きさ
int  : width 横の大きさ
int  : agent_num agentの人数
int  : turn 今の残りターン数
int  : points[i][j] x座標:j, y座標:i のマスの点数
int  : tiled[i][j] x座標:j, y座標:i のマスの状態
int  : agent_exist[i][j] x座標:j, y座標:i のマスにいるエージェント
int  : hoge_score hogeの点数(領域ポイントは含まない
int  : hoge_area_score hogeの領域ポイント(純粋なマスの点数は含まない

tiled, point, agent_existなどの配列の値はteamID(後述)

構造体
agent_info
agent1体の情報を持っている

int    : agentID agentの固有ID
int    : y/x y/x座標の位置
int    : next_y/x 次に行動を行う座標のy/x座標
string : next_command "move","remove"で、行動の種類を決定する。

next_commandがemptyでなく、文字が"move"or"remove"だった時にのみ行動を実行する


team_info
チームごとの情報を持っている
int        : teamID  team固有のID
int        : tilePoint 名前の通り　領域は含めない
int        : areaPoint 名前の通り
agent_info : agent[8] それぞれのagentの情報が入っている

action_command
本番環境でサーバーから帰ってくる情報(だったはず)
今は使っていない(使うかもしれないので取っておいてある)

行動の仕方
上述の通りnext_hogeにいい感じ値を入れれば動きます
input_next_commandという関数でも値を入れることができます
agent_info.input_next_command(Y,X,command)で大丈夫です

input_random_actionというランダムにエージェントを動かす行動を入力する関数を作ったのでこれを見ればわかると思います


その他注釈
基本的に変数のteam_color,また、tiled, point, agent_existなどの配列の値もteamIDで区別されています



ctrl+z 一つ戻る//未実装
ctrl+r リセット

得点の低いタイルを消去することができる(色がつかなくなる)
上矢印キー タイル消去のボーダーを上げる
下矢印キー タイル消去のボーダーを下げる


