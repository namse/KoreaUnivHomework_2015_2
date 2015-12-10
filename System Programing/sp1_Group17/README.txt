README

blk-core.c
modules/
-Makefile
-simplemodule.c
sp1_Group17.XLSX

1. blk-core.c
리눅스 커널/block/blk-core.c 에 해당하는 항목.
submit_bio function 쪽이 수정되어있음.

2. module/Makefile
simplemodule.c를 컴파일 할 Makefile.
커널 DIR은 타이젠에 올려진 리눅스의 DIR로 셋팅해야 함.

3. module/simplemodule.c
procfs를 만들 모듈. circular 버퍼를 만들지 않고 충분히 큰 버퍼를 생성해서 그 곳에 로그 작성 후 모듈 재 삽입을 통해 데이터 비움.

4. sp1_Group17
결과물이 들어있는 엑셀자료. 그래프 등 자료는 보고서를 통해 봐주시면 감사드리겠습니다.

자세한 한글 주석은 보고서에 추가로 달아놓았습니다.
