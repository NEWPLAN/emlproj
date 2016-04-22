#include <stdio.h>
#include <string.h>
#include <cppuhelper/bootstrap.hxx>
#include <osl/file.hxx>
#include <osl/process.h>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/sheet/XCellRangesQuery.hpp>
#include <com/sun/star/sheet/CellFlags.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/text/XTextFrame.hpp>
#include <com/sun/star/text/XTextFramesSupplier.hpp>
#include <com/sun/star/style/XStyleFamiliesSupplier.hpp>
#include <com/sun/star/container/XNameContainer.hpp>
#include <com/sun/star/drawing/XDrawPagesSupplier.hpp>
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::frame;
using namespace com::sun::star::sheet;
using namespace com::sun::star::text;
using namespace com::sun::star::drawing;
using namespace com::sun::star::container;
using namespace com::sun::star::table;
using namespace com::sun::star::style;
using namespace rtl;
using namespace cppu;
using rtl::OUString;
using rtl::OString;
using namespace osl;
using namespace std;

#ifdef __cplusplus
extern "C"
{
#endif

int docmain(char* src, char* workspace,char *curpath)
{

    OUString sDocAddr, sDocUrl;
    char srcpath[1024]= {0};
    char destpath[1024]= {0};
    sprintf(srcpath,"%s/%s/%s",workspace,curpath,src);
    sprintf(destpath,"%s/temps/doc.txt",workspace);
    FILE *fp;
    fp = fopen(destpath,"ab");
    if(fp == NULL)
    {
        printf("open result_file failed, with source office file:%s\n",destpath);
        return -1;
    }
    printf("in .doc processing...\n");

    sDocAddr = OUString(srcpath, strlen(srcpath), RTL_TEXTENCODING_UTF8);
    osl::FileBase::getFileURLFromSystemPath(sDocAddr, sDocUrl);
    Reference<XComponentContext>		xContext;
    Reference<XMultiComponentFactory>	xMSFactory;
    Reference<XComponent>			    xDocument;

    xContext = bootstrap();
    xMSFactory = xContext->getServiceManager();
    Reference<XInterface> desktop = xMSFactory->createInstanceWithContext(OUString::createFromAscii("com.sun.star.frame.Desktop"), xContext);
    Reference<XComponentLoader> rComponentLoader(desktop, UNO_QUERY_THROW);
    Sequence<PropertyValue> loadProps(1);
    loadProps[0].Name = OUString::createFromAscii("Hidden");
    loadProps[0].Value = Any(true);
    try
    {
        xDocument = rComponentLoader->loadComponentFromURL(sDocUrl, OUString(RTL_CONSTASCII_USTRINGPARAM("_blank")), 0, loadProps);
    }
    catch(Exception &e)
    {
        printf("	can't open %s\n", srcpath);
        return -1;
    }

    printf("	after open office_file.\n");

    //提取文档主体内容以及表格内容
    Reference<XTextDocument> rTextDoc(xDocument, UNO_QUERY);
    Reference<XText> rText = rTextDoc->getText();
    OUString rTextStr = rText->getString();
    OString toPrintOut = OUStringToOString(rTextStr, RTL_TEXTENCODING_UTF8);
    //printf( "Text in doc is :\n%s\n", toPrintOut.pData->buffer );
    fwrite(toPrintOut.pData->buffer, strlen(toPrintOut.pData->buffer), 1, fp);

    //提取文本框中内容
    Reference<XTextFramesSupplier> rTFS(xDocument, UNO_QUERY);
    Reference<XNameAccess> rTextFrames = rTFS->getTextFrames();
    Sequence<OUString> frameNames = rTextFrames->getElementNames();
    int frameNum = frameNames.getLength();
    //printf("There're %d frames\n", frameNum);
    for(int i = 0; i < frameNum; i++)
    {
        Any tfTemp = rTextFrames->getByName(frameNames[i]);
        Reference<XTextFrame> rTextFrame(tfTemp, UNO_QUERY);
        Reference<XText> rToF = rTextFrame->getText();
        OUString tempStr = rToF->getString();
        tempStr += OUString::createFromAscii("\n");
        OString printStr = OUStringToOString(tempStr, RTL_TEXTENCODING_UTF8);
        //printf("Text in frame %d is:\n%s\n\n", i, printStr.pData->buffer);
        fwrite(printStr.pData->buffer, strlen(printStr.pData->buffer), 1, fp);
    }

    //提取页眉页脚内容
    Reference<XStyleFamiliesSupplier> rSFS(rTextDoc, UNO_QUERY);
    Reference<XNameAccess> rStyleFamilies = rSFS->getStyleFamilies();
    Reference<XNameContainer> pageStyles(rStyleFamilies->getByName(OUString::createFromAscii("PageStyles")), UNO_QUERY);
    Reference<XPropertySet> pageProps(pageStyles->getByName(OUString::createFromAscii("Standard")), UNO_QUERY);
    bool hasHeader, hasFooter;
    Reference<XText> headerText, footerText;
    pageProps->getPropertyValue(OUString::createFromAscii("HeaderIsOn")) >>= hasHeader;
    pageProps->getPropertyValue(OUString::createFromAscii("FooterIsOn")) >>= hasFooter;
    if(hasHeader)
    {
        pageProps->getPropertyValue(OUString::createFromAscii("HeaderText")) >>= headerText;
        //printf("headerText:\n%s\n\n", OUStringToOString(headerText->getString(), RTL_TEXTENCODING_UTF8).pData->buffer);
        OUString headerOUStr = headerText->getString();
        headerOUStr += OUString::createFromAscii("\n");
        OString headerStr = OUStringToOString(headerOUStr, RTL_TEXTENCODING_UTF8);
        fwrite(headerStr.pData->buffer, strlen(headerStr.pData->buffer), 1, fp);
    }
    if(hasFooter)
    {
        pageProps->getPropertyValue(OUString::createFromAscii("FooterText")) >>= footerText;
        OUString footerOUStr = footerText->getString();
        footerOUStr += OUString::createFromAscii("\n");
        OString footerStr = OUStringToOString(footerOUStr, RTL_TEXTENCODING_UTF8);
        fwrite(footerStr.pData->buffer, strlen(footerStr.pData->buffer), 1, fp);
        //printf("footerText:\n%s\n\n", OUStringToOString(footerText->getString(), RTL_TEXTENCODING_UTF8).pData->buffer);
    }
    xDocument->dispose();
    fclose(fp);
    printf("	%s finished.\n", srcpath);
    return 0;
}

int xlsmain(char* src, char* workspace,char *curpath)
{
    char srcpath[1024]= {0};
    char destpath[1024]= {0};
    sprintf(srcpath,"%s/%s/%s",workspace,curpath,src);
    sprintf(destpath,"%s/temps/xls.txt",workspace);

    OUString sDocAddr, sDocUrl;
    FILE *fp;
    fp = fopen(destpath,"ab");
    if(fp == NULL)
    {
        printf("open result_file failed, with source office file:%s\n", srcpath);
        return -1;
    }

    //提取每张表格中的内容
    printf("in .xls processing...\n");
    sDocAddr = OUString(srcpath, strlen(srcpath), RTL_TEXTENCODING_UTF8);
    osl::FileBase::getFileURLFromSystemPath(sDocAddr, sDocUrl);
    Reference<XComponentContext>		xContext;
    Reference<XMultiComponentFactory>	xMSFactory;
    Reference<XComponent>			    xDocument;

    xContext = bootstrap();
    xMSFactory = xContext->getServiceManager();
    Reference<XInterface> desktop = xMSFactory->createInstanceWithContext(
                                        OUString::createFromAscii("com.sun.star.frame.Desktop"),
                                        xContext);
    Reference<XComponentLoader> rComponentLoader(desktop, UNO_QUERY_THROW);
    Sequence<PropertyValue> loadProps(1);
    loadProps[0].Name = OUString::createFromAscii("Hidden");
    loadProps[0].Value = Any(true);
    try
    {
        xDocument = rComponentLoader->loadComponentFromURL(sDocUrl, OUString(RTL_CONSTASCII_USTRINGPARAM("_blank")), 0, loadProps);
    }
    catch(Exception &e)
    {
        printf("	can't open %s\n", srcpath);
        return -1;
    }

    printf("	after open office_file.\n");
    Reference<XSpreadsheetDocument> rSheetDoc(xDocument, UNO_QUERY);
    Reference<XNameAccess> rSheets(rSheetDoc->getSheets(), UNO_QUERY);
    Sequence<OUString> sheetNames = rSheets->getElementNames();
    int sheetNum = sheetNames.getLength();
    //printf("Sheet Num : %d\n\n", sheetNum);
    for(int i = 0; i < sheetNum; i++)
    {
        //printf("Sheet %d {\n\n", i+1);
        Reference<XSpreadsheet> rSheet(rSheets->getByName(sheetNames[i]), UNO_QUERY);
        Reference<XCellRangesQuery> rCRQ(rSheet, UNO_QUERY);
        Reference<XSheetCellRanges> rCellRanges = rCRQ->queryContentCells((short)(CellFlags::STRING + CellFlags::VALUE + CellFlags::DATETIME + CellFlags::FORMULA));
        Sequence<CellRangeAddress> rangeAddrs = rCellRanges->getRangeAddresses();
        int addrsNum = rangeAddrs.getLength();
        for(int j = 0; j < addrsNum; j++)
        {
            CellRangeAddress rangeAddr = rangeAddrs[j];
            for(int row = rangeAddr.StartRow; row <= rangeAddr.EndRow; row++)
            {
                for(int col = rangeAddr.StartColumn; col <= rangeAddr.EndColumn; col++)
                {
                    Reference<XCell> rCell = rSheet->getCellByPosition(col, row);
                    OUString content = rCell->getFormula();
                    content += OUString::createFromAscii("\n");
                    OString contOStr = OUStringToOString(content, RTL_TEXTENCODING_UTF8);
                    fwrite(contOStr.pData->buffer, strlen(contOStr.pData->buffer), 1, fp);
                    //printf("[%s] ", OUStringToOString(content, RTL_TEXTENCODING_UTF8).pData->buffer);
                }
            }
        }
    }
    xDocument->dispose();
    fclose(fp);
    printf("	%s finished.\n", srcpath);
    return 0;
}


int pptmain(char* src, char* workspace,char *curpath)
{
    char srcpath[1024]= {0};
    char destpath[1024]= {0};
    sprintf(srcpath,"%s/%s/%s",workspace,curpath,src);
    sprintf(destpath,"%s/temps/ppt.txt",workspace);

    OUString sDocAddr, sDocUrl;
    FILE *fp;
    fp = fopen(destpath,"ab");
    if(fp == NULL)
    {
        printf("open result_file failed, with source office file:%s\n", srcpath);
        return -1;
    }

    printf("in .ppt processing...\n");
    sDocAddr = OUString(srcpath, strlen(srcpath), RTL_TEXTENCODING_UTF8);
    osl::FileBase::getFileURLFromSystemPath(sDocAddr, sDocUrl);
    Reference<XComponentContext>		xContext;
    Reference<XMultiComponentFactory>	xMSFactory;
    Reference<XComponent>			    xDocument;

    xContext = bootstrap();
    xMSFactory = xContext->getServiceManager();
    Reference<XInterface> desktop = xMSFactory->createInstanceWithContext(
                                        OUString::createFromAscii("com.sun.star.frame.Desktop"),
                                        xContext);
    Reference<XComponentLoader> rComponentLoader(desktop, UNO_QUERY_THROW);
    Sequence<PropertyValue> loadProps(1);
    loadProps[0].Name = OUString::createFromAscii("Hidden");
    loadProps[0].Value = Any(true);
    try
    {
        xDocument = rComponentLoader->loadComponentFromURL(sDocUrl, OUString(RTL_CONSTASCII_USTRINGPARAM("_blank")), 0, loadProps);
    }
    catch(Exception &e)
    {
        printf("	can't open %s\n", srcpath);
        return -1;
    }

    printf("	after open office_file.\n");
    Reference<XDrawPagesSupplier> rDPS(xDocument, UNO_QUERY);
    Reference<XDrawPages> rDP = rDPS->getDrawPages();
    //printf("There're %d slides.\n", rDP->getCount());
    int slidesNum = rDP->getCount();
    OUString emptyString = OUString::createFromAscii("");
    for(int j = 0; j < slidesNum; j++)
    {
        Reference<XDrawPage> slide;
        rDP->getByIndex(j) >>= slide;
        int shapesNum = slide->getCount();
        for(int i = 0; i < shapesNum; i++)
        {
            Reference<XShape> rShape;
            slide->getByIndex(i) >>= rShape;
            //OUString shapeType = rShape->getShapeType();
            //printf("        Shape%d's type is : %s\n", i+1, OUStringToOString(shapeType,RTL_TEXTENCODING_UTF8).pData->buffer);

            Reference<XText> rText(rShape, UNO_QUERY);
            if(rText != NULL)
            {
                OUString textInShape = rText->getString();
                if(!textInShape.equals(emptyString))
                {
                    textInShape += OUString::createFromAscii("\n");
                    OString textOStr = OUStringToOString(textInShape, RTL_TEXTENCODING_UTF8);
                    fwrite(textOStr.pData->buffer, strlen(textOStr.pData->buffer), 1, fp);
                    //printf("    Text in shape%d : %s\n", i+1, OUStringToOString(textInShape, RTL_TEXTENCODING_UTF8).pData->buffer);
                }
            }
        }
    }
    xDocument->dispose();
    fclose(fp);
    printf("	%s finished.\n", srcpath);
    return 0;
}


#ifdef __cplusplus
}
#endif
