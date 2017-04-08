if exist grpc (
REM xcopy .gitmodules grpc /D /Y
xcopy .gitignore grpc /D /Y
cd grpc
git fetch --all --tags --prune
git checkout tags/v1.2.3
echo "grpc release tags: "
git describe --tags
git submodule update --init --remote --merge --recursive
rem cd third_party\protobuf
rem git fetch --all --tags --prune
rem git checkout v3.2.0
rem echo "protobuf release tags: "
rem git describe --tags
) else (
git clone --depth 1 --branch v1.2.3 --single-branch https://github.com/grpc/grpc.git
git checkout tags/v1.2.3
echo "grpc release tags: "
git describe --tags
REM xcopy .gitmodules grpc/ /D /Y
xcopy .gitignore grpc /Y
cd grpc
git submodule update --init --remote --merge --recursive
rem cd third_party\protobuf
rem git fetch --all --tags --prune
rem git checkout v3.2.0
rem echo "protobuf release tags: "
rem git describe --tags
REM git config core.sparseCheckout true
REM echo src/* >> .git/info/sparse-checkout
REM echo include/* >> .git/info/sparse-checkout
REM echo third_party/* >> .git/info/sparse-checkout
REM git checkout master
)

cd ..\..\..