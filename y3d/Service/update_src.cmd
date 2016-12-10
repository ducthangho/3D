if exist grpc (
xcopy .gitmodules grpc /D /Y
xcopy .gitignore grpc /D /Y
cd grpc
git pull origin master
git submodule update --recursive --remote
) else (
git clone --no-checkout --depth 1 https://github.com/grpc/grpc.git
xcopy .gitmodules grpc/ /D /Y
xcopy .gitignore grpc/ /D /Y
cd grpc
git config core.sparseCheckout true
echo src/* >> .git/info/sparse-checkout
echo include/* >> .git/info/sparse-checkout
echo third_party/* >> .git/info/sparse-checkout
git checkout master
)

cd ..