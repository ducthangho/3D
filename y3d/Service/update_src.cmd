if exist grpc (
REM xcopy .gitmodules grpc /D /Y
xcopy .gitignore grpc /D /Y
cd grpc
git fetch --all --tags --prune
git checkout tags/v1.1.2
git submodule update --init --remote --merge --recursive
) else (
git clone --depth 1 --branch v1.1.2 --single-branch https://github.com/grpc/grpc.git
git checkout tags/v1.1.2
REM xcopy .gitmodules grpc/ /D /Y
xcopy .gitignore grpc /Y
cd grpc
git submodule update --init --remote --merge --recursive
REM git config core.sparseCheckout true
REM echo src/* >> .git/info/sparse-checkout
REM echo include/* >> .git/info/sparse-checkout
REM echo third_party/* >> .git/info/sparse-checkout
REM git checkout master
)

cd ..