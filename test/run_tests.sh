cd build 
pass=0
fail=0
RED='\033[0;31m'
GREEN='\033[0;32m'
function check_status() {
    if [ $? -eq 0 ];then
        ((pass++))
    else    
        ((fail++))
    fi
}

function run_test() {
    $1
    check_status
}

run_test "./testmain --gtest_filter=BytecodesTest*"
run_test "./testmain --gtest_filter=FenwickTreeTest*"
run_test "./testmain --gtest_filter=QuFrequencyTest*"
run_test "./testmain --gtest_filter=QuWorldTest*"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state2test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state3test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state4test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state5test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state6test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state7test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state8test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state9test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state10test"
run_test "./testmain --gtest_filter=BasicMeasurementTest.state_calltest"
run_test "./testmain --gtest_filter=SwapTest.fullSwap"
run_test "./testmain --gtest_filter=BasicMeasurementTest.abSwap"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.cnotState1"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.cnotState2"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.semiCnotState3"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.crotState1"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.semiCrotState1"
run_test "./testmain --gtest_filter=CoherentMeasurementTest.cohQft24"
run_test "./testmain --gtest_filter=QFTTEST.qft12"
run_test "./testmain --gtest_filter=QFTTEST.qft13"
run_test "./testmain --gtest_filter=QFTTEST.qft14"
run_test "./testmain --gtest_filter=QFTTEST.qft23"
run_test "./testmain --gtest_filter=QFTTEST.qft24"
run_test "./testmain --gtest_filter=QFTTEST.qft34"
run_test "./testmain --gtest_filter=IQFTTEST.iqft12"
run_test "./testmain --gtest_filter=IQFTTEST.iqft13"
run_test "./testmain --gtest_filter=IQFTTEST.iqft14"
run_test "./testmain --gtest_filter=IQFTTEST.iqft23"
run_test "./testmain --gtest_filter=IQFTTEST.iqft24"
run_test "./testmain --gtest_filter=IQFTTEST.iqft34"
run_test "./testmain --gtest_filter=QFTIntrinsicsTest.qft34Intrinsic"
run_test "./testmain --gtest_filter=QFTIntrinsicsTest.iqft34Intrinsic"

printf "\n\n"
echo -e "${GREEN}PASS: $pass    ${RED}FAIL: $fail"
printf "\n"

if [ $fail -eq 0 ];then 
    echo -e "${GREEN} ALL TEST PASSED"
    printf "\n"
    exit 0
else 
    echo -e "${RED} THERE ARE $fail FAILURES"
    printf "\n"
    exit 1
fi
