if exist grpc (
REM xcopy .gitmodules grpc /D /Y
xcopy .gitignore grpc /D /Y
cd grpc
git fetch
git checkout v1.0.s
git submodule update --init --recursive
git submodule update --remote
) else (
git clone --depth 1 --branch v1.0.s --single-branch https://github.com/grpc/grpc.git
REM xcopy .gitmodules grpc/ /D /Y
xcopy .gitignore grpc /Y
cd grpc
git submodule update --init --recursive
git submodule update --remote
REM git config core.sparseCheckout true
REM echo src/* >> .git/info/sparse-checkout
REM echo include/* >> .git/info/sparse-checkout
REM echo third_party/* >> .git/info/sparse-checkout
REM git checkout master
)

cd ..